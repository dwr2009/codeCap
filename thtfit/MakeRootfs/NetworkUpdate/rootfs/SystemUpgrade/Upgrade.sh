#!/bin/sh

# Upgrade.sh, V1.6.0
# set -x

PATH=/bin:/sbin:/usr/bin:/usr/sbin
RootfsDir=/mnt/rootfs

UpgradeFilesSrcDir="$RootfsDir/SystemUpgrade"

UpgradeFilesTempDir="/tmp/SystemUpgrade"

SystemSettingsBackupDir=/tmp/SysSetBackup
CorrectFileListToBackupFile=${SystemSettingsBackupDir}/FilesToBackup.txt
SysSetBackupPackageFile=${SystemSettingsBackupDir}/SysSetBackupPackage.tar

# Image files.
Zboot="bolt.bin"
Kernel="zImage"
Rootfs="rootfs.tgz"

declare -a images=( $Zboot $Kernel $Rootfs )
declare -a imagesName=( Zboot Kernel Rootfs )

Mp7xxDefaultConfigFile="Mp7xxDefCfg.tgz"
Mp7xxDefaultLocalMediaFile="Mp7xxLocalMedia.tgz"

function ExitProcess()
{
	ArgCnt=$#
	Arg1=$1
	#echo ${ArgCnt}
	local bIsShellEnv=0
	
	if [ "${LOGNAME}" != "" ]; then
		bIsShellEnv=1
	fi

	if [ ${ArgCnt} -gt 0 ]; then
		if [ ${bIsShellEnv} -ne 0 ]; then
			exit ${Arg1}
		else
			exec sh
		fi
	else
		if [ ${bIsShellEnv} -ne 0 ]; then
			exit
		else
			exec sh
		fi
	fi
}

#$1:input string $2:input time $3:input Color
do_checkchipid()
{
    local chipid_current
    UpgradeClient "Check chip pid..."
    chipid_current=`cat /proc/cpuinfo|grep "Platform"|sed 's/^Platform\s*:\s*//g'|sed 's/\s*$//g'`
    if [ -z ${chipid_current} ];then
        chipid_current=`dmesg | grep "model:" | awk -F 'model:' '{ print $2 }' | awk -F ' ' '{ print $1 }'`
    fi

    if [ -f "$UpgradeFilesTempDir/chipid" ]; then
        chipid_package=$(cat $UpgradeFilesTempDir/chipid)
        if [ "$chipid_package" = "$chipid_current" ]; then
	    UpgradeClient "Check chip pid successfully." 
            return 0
        fi
        UpgradeClient "ERROR: The upgrading package is used for $chipid_package solution and not compatible with current device of $chipid_current solution."
    else
        UpgradeClient "ERROR: chipid file does not exist."
    fi

    return 1
}

do_checksum()
{
    UpgradeClient "MD5 Checksum ..."
    cd $UpgradeFilesTempDir
    md5sum -c md5sum
    if [ $? -ne 0 ]; then
        UpgradeClient "ERROR: MD5sum incorrect, must cancel the upgrade."
        return 1
    fi

    sleep 1

    UpgradeClient "MD5 Checksum PrintInfo successfully"
    return 0
}

function deletfile()
{
        local dfile=$1
        if [ -d "$dfile" ]; then
                rm -fr $dfile
                ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "Delete $dfile err ${ProcessExitCode}"                                                                                       
                fi
        else
                rm -f $dfile
                ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "Delete $dfile err ${ProcessExitCode}"                                                                                       
                fi
        fi
        return 0
}


function formatMTD()
{
        local UBIMTD=$1
        flash_eraseall  --noskipbad ${UBIMTD}
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                echo "flash_eraseall(${UBIMTD}) err ${ProcessExitCode}"
                mountflag=1
        fi

        nMtdSubPageSize=`mtdinfo ${UBIMTD}|grep "Sub-page size"|awk -F ':' '{ print $2 }' | awk -F ' ' '{ print $1 }'`
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                echo "GetSubPageSize(${UBIMTD}) err ${ProcessExitCode}"                                               
                nMtdSubPageSize=8192
        fi

        nMinIoUnitSize=`mtdinfo ${UBIMTD} | grep "Minimum input/output unit size" | awk -F ':' '{ print $2 }' | awk -F ' ' '{ print $1 }'`
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                echo "GetMinIoUnitSize(${UBIMTD}) err ${ProcessExitCode}"                                                
                nMinIoUnitSize=8192
        fi

        ubiformat -y --sub-page-size=${nMtdSubPageSize} --vid-hdr-offset=${nMinIoUnitSize} $UBIMTD
        return 0
}

function attachUBI()
{
        local UBIMTD=$1
	local MOUNTMTD=$2
        local iOutRet=0

        tries=5
        while [ $tries -ne 0 ]; do
                ubiattach -p $UBIMTD
                if [ 0 = $? ]; then
                        break
                fi
		
		mount | grep "${MOUNTMTD}"
		if [ 0 = $? ]; then
			umount  ${MOUNTMTD}
		fi

                ubidetach -p $UBIMTD
                if [ 0 = $? ]; then
                        continue                                                                                                                           
                fi

                formatMTD $UBIMTD
                tries=$(($tries-1))
        done

        if [ $tries -eq 0 ]; then
                iOutRet=-1
        fi

        return $iOutRet

}


function mountUBI()
{
        local mtdname=$1
        local mountname=$2
        local UBIMTD
        if [ ! -d "$mountname" ]; then
                mkdir -p $mountname
        fi
        UBIMTD="`cat /proc/mtd | grep $mtdname | head -c 4`"
        partnum=`echo  "$UBIMTD" | cut -d "d" -f2`
	if [ "$UBIMTD" != "" ]; then
                UBIMTD="/dev/$UBIMTD"
                if [ -e $UBIMTD ]; then
                        attachUBI $UBIMTD $mountname
                        if [ 0 = $? ]; then
                                UBIVolumesName="`ubinfo -a | grep "Name: *$mtdname"`"
                                if [ "${UBIVolumesName}" = "" ]; then
                                	ubimkvol /dev/ubi$partnum -N $mtdname --maxavsize
                                        sync
                                        sleep 3
                                fi
                        fi
                fi
        fi
	mount -t ubifs -o chk_data_crc ubi$partnum:$mtdname $mountname
        return $?

}

function umountUBI()
{
        local mtdname=$1
        local mountname=$2
        UBIMTD="`cat /proc/mtd | grep $mtdname | head -c 4`"                                                                                            
        UBIMTD="/dev/$UBIMTD"
        umount $mountname &>/dev/null
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                return -1
        fi
        sync
        ubidetach -p $UBIMTD
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                return -1
        fi

        return 0
}


do_zboot()
{
	local processExt=0
	local filesize=0
	local tries
	local deviceMTD
	local partnum	
	UpgradeClient "Upgrade $Zboot......"
	deviceMTD="`cat /proc/mtd | grep "flash0.bolt" | head -c 4`"
        partnum=`echo  "$deviceMTD" | cut -d "d" -f2`
        deviceMTD=/dev/mtdblock$partnum	
	if [ -f $UpgradeFilesTempDir/bootUpgradeFlagFile ]; then
		bflag=`cat $UpgradeFilesTempDir/bootUpgradeFlagFile`
		if [ bflag ]; then
			UpgradeClient "bootload have alreadly upgrade."
			rm -rf $UpgradeFilesTempDir/bootUpgradeFlagFile
			return 0
		fi
	fi

	filesize=`ls -l $UpgradeFilesTempDir/$Zboot | awk '{print $5}'`
	tries=10
	while [ $tries -ne 0 ]; do
		dd if=$deviceMTD of=/tmp/tmp.bin bs=$filesize count=1 &>/dev/null
		cmp ${UpgradeFilesTempDir}/$Zboot /tmp/tmp.bin &>/dev/null
		if [ $? -eq 0 ]; then                                                                                                                  
           		if [ $tries -eq 10 ]; then
				UpgradeClient "bootload is current version."                                                                                                                          
        		else
				UpgradeClient "Load bootload successfully."
				echo 1 > $UpgradeFilesTempDir/bootUpgradeFlagFile				
				sleep 1
				UpgradeClient "update new bootload need restat..."
				reboot
			fi
			break
		fi
		dd if=$UpgradeFilesTempDir/$Zboot of=$deviceMTD	 bs=$filesize count=1 &>/dev/null
		tries=$(($tries-1))		
	done
	
	if [ $tries -eq 0 ]; then                                                                                                                       
       		processExt=-1                                                                                                                               
    	fi

    	return $processExt
}

do_kernel()
{
    local image
    image=$1
    local processExt=0
    local var=0
    local deviceMTD

    deviceMTD="`cat /proc/mtd | grep "flash0.kernel" | head -c 4`"
    partnum=`echo  "$deviceMTD" | cut -d "d" -f2`
    deviceMTD=/dev/mtdblock$partnum
    var=`ls -l ${image} | awk '{print $5}'` 
    tries=10                                                                                                                               
    while [ $tries -ne 0 ]; do                                                                                                              
        dd if=$deviceMTD of=/tmp/tmp.bin bs=$var count=1 &>/dev/null                                                                    
        cmp ${image} /tmp/tmp.bin &>/dev/null                                                                                     
        if [ $? -eq 0 ]; then                                                                                                               
            if [ $tries -eq 10 ]; then                                 
                UpgradeClient "Your linux kernel version is current."                                                                                                                   
            else                                                                                                                                                         UpgradeClient "$image is successfully upgraded."
            fi                                              
             break                                 
        fi                                                                                                                                  
        
        dd if=${image} of=$deviceMTD &>/dev/null          
        sleep 1                                               
        tries=$(($tries-1))                             
    done                                                               
 
    if [ $tries -eq 0 ]; then 
	processExt=-1                                     
    fi                                                                                                                                                                              
    return $processExt
}

do_rootfs()
{
	local image
	local UBIMTD
	local attachSuccess
	image=$1
	mountUBI "rootfs" "/mnt/rootfs"
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                return -1
        fi
        ROOTFS_DIR=/mnt/rootfs                                                                                                                          
        cd ${ROOTFS_DIR}       
	deletfile "${ROOTFS_DIR}/mnt"
        deletfile "${ROOTFS_DIR}/LocalMedia"
        deletfile "${ROOTFS_DIR}/Config"
        deletfile "${ROOTFS_DIR}/data"
	DeleteOldFiles "${ROOTFS_DIR}"                                                                                                      
	BackupCurSystemSettings "${ROOTFS_DIR}"                                        
	zcat ${image} | tar xpf -                                            
        res=$?                                                                         
        if [ $res -ne 0 ]; then                                                        
        	UpgradeClient "ERROR: extracting file failed."  
		sleep 3
		return -1                                                                                                                               
        else                                                                                                                                            
        	UpgradeClient "$image is successfully upgraded."                                    
        fi
	
	if [ -f ${UpgradeFilesSrcDir}/${Mp7xxDefaultConfigFile} ]; then   
		mountUBI "config" "/mnt/config"
		ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "mount config err ${ProcessExitCode}"
                fi
                    
		if [ ! -d /mnt/config/Config ]; then
                        mkdir /mnt/config/Config
                fi

		if [ -h ${ROOTFS_DIR}/MP7XX/Config ];then
                        mv ${ROOTFS_DIR}/MP7XX/Config ${ROOTFS_DIR}/MP7XX/Config.bak
                        ln -s /mnt/config/Config ${ROOTFS_DIR}/MP7XX/Config
                fi
		umconfig=1
        	DirToCntFileNum=${ROOTFS_DIR}/MP7XX/Config                         
        	if [ -h ${DirToCntFileNum} ]; then                                                                                                      
                	FileNumCnt=`ls ${DirToCntFileNum} | wc -l`                         
               		ProcessExitCode=$?                                                 
               		if [ ${ProcessExitCode} -ne 0 ]; then                                                                                        
                        	echo "GetFileNumCnt(${DirToCntFileNum}) err ${ProcessExitCode}"                                                          
                      	fi                                                                      
      		else                                                                       
                        FileNumCnt=0                                                           
       		fi                                                                         
                                                                                               
		if [ ${FileNumCnt} -eq 0 ]; then                                                                                                        
                	UpgradeClient "Unpacking ${Mp7xxDefaultConfigFile}"                                                                             
                	zcat ${UpgradeFilesSrcDir}/${Mp7xxDefaultConfigFile} | tar -xf -                                                                
                	ProcessExitCode=$?                                                                                                              
                	if [ ${ProcessExitCode} -ne 0 ]; then                                                                                                                               
                        	echo "UnpackFile(${Mp7xxDefaultConfigFile}) err ${ProcessExitCode}"                                                     
                        fi                                                                     
                fi  

		if [ -h ${ROOTFS_DIR}/MP7XX/Config.bak ];then
                        rm -rf ${ROOTFS_DIR}/MP7XX/Config
                        mv ${ROOTFS_DIR}/MP7XX/Config.bak ${ROOTFS_DIR}/MP7XX/Config
                fi                                                                       
	fi                                                                             
                                                                                               
        if [ -f ${UpgradeFilesSrcDir}/${Mp7xxDefaultLocalMediaFile} ]; then 
		mountUBI "data" "/mnt/data"
		ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "mount data err ${ProcessExitCode}"
                fi
		umdata=1
		if [ ! -d /mnt/data/LocalMedia ];then
                        mkdir /mnt/data/LocalMedia
                fi

		if [ ! -d /mnt/data/data ];then
                        mkdir /mnt/data/data
                fi

		if [ -h ${ROOTFS_DIR}/MP7XX/LocalMedia ];then
                        mv ${ROOTFS_DIR}/MP7XX/LocalMedia ${ROOTFS_DIR}/MP7XX/LocalMedia.bak
                        ln -s /mnt/data/LocalMedia ${ROOTFS_DIR}/MP7XX/LocalMedia
                fi
                       
        	DirToCntFileNum=${ROOTFS_DIR}/MP7XX/LocalMedia                                                                                         
        	if [ -h ${DirToCntFileNum} ]; then                                                                                                     
        		FileNumCnt=`find ${DirToCntFileNum}/ -type f | wc -l`                   
                	ProcessExitCode=$?                                                                                                          
			if [ ${ProcessExitCode} -ne 0 ]; then                                                                                       
                       		echo "GetFileNumCnt(${DirToCntFileNum}) err ${ProcessExitCode}"                                                     
                        fi                                                                                                                          
               	else                                                                                                                           
                	FileNumCnt=0                                                               
                fi                                                                            
                                                                                                   
                if [ ${FileNumCnt} -eq 0 ]; then                                          
                    UpgradeClient "Unpacking ${Mp7xxDefaultLocalMediaFile}"                                                                             
                    zcat ${UpgradeFilesSrcDir}/${Mp7xxDefaultLocalMediaFile} | tar -xf -                                                                
                    ProcessExitCode=$?                                                         
                    if [ ${ProcessExitCode} -ne 0 ]; then                                                                                       
                        echo "UnpackFile(${Mp7xxDefaultLocalMediaFile}) err ${ProcessExitCode}"                                                 
                    fi                                                                         
                fi      
	
		if [ -h ${ROOTFS_DIR}/MP7XX/LocalMedia.bak ];then
                        rm -rf ${ROOTFS_DIR}/MP7XX/LocalMedia
                        mv ${ROOTFS_DIR}/MP7XX/LocalMedia.bak ${ROOTFS_DIR}/MP7XX/LocalMedia
                fi		
                                                                       
	fi

	ResotoreCurSystemSettings          
	cd /
	echo "" > $RootfsDir/update_is_successful                                            
	DeleteTempBackupFiles     
	deletfile "${ROOTFS_DIR}/Temp"   
	rm -fr $UpgradeFilesSrcDir
	rm -f $RootfsDir/update_is_successful

	sync
        sleep 1
	if [ ${umconfig} -eq 1 ]; then
                umountUBI "config" "/mnt/config"
                ProcessExitCode=$?                                                                                                                      
                if [ ${ProcessExitCode} -ne 0 ]; then                                                                                                   
                        echo "umount config err ${ProcessExitCode}"                                                                                     
                fi
        fi

        if [ ${umdata} -eq 1 ]; then
                umountUBI "data" "/mnt/data"
                ProcessExitCode=$?                                                                                                                      
                if [ ${ProcessExitCode} -ne 0 ]; then                                                                                                   
                        echo "umount data err ${ProcessExitCode}"                                                                                      
                fi
        fi

	return 0
}

function createEMMCPART
{
    local devname=$1
    
    partname="rootfs"
    mount | grep -n /mnt/${partname}
    if [ $? -eq 0 ];then
        umount /mnt/${partname}
    fi
    
    
    dd if=/dev/zero  of=${devname}  bs=512 count=128
    sgdisk -o ${devname}
    sleep 3
    sgdisk -g -a 1 -n 1:34:1228834 -c 1:"${partname}" ${devname}
    ProcessExitCode=$?
    if [ ${ProcessExitCode} -ne 0 ]; then
        echo "create partition ${partname} on ${devname} failed" 
        return $ProcessExitCode
    fi
    sleep 3
    
    if [ ! -e ${devname}p1 ];then
        echo "do not exist ${devname}p1"
        return -1
    fi
        
    mkfs.ext4 ${devname}p1
    ProcessExitCode=$?
    if [ ${ProcessExitCode} -ne 0 ]; then                                           
        echo "mkfs.ext4 ${devname}p1 error."
        return -1                               
    fi 
    
    partname="config"   
    sgdisk -g -a 1 -n 2:1228835:1843234 -c 2:"${partname}" ${devname}
    ProcessExitCode=$?
    if [ ${ProcessExitCode} -ne 0 ]; then
        echo "create config error:${ProcessExitCode}"
        return "$ProcessExitCode"
    fi
        
    sleep 3                                                                                
    if [ ! -e ${devname}p2 ];then                                                          
        echo "do not exist ${devname}p2"                                               
        return -1                                                                      
    fi

    mkfs.ext4 ${devname}p2
    ProcessExitCode=$?                                            
    if [ ${ProcessExitCode} -ne 0 ]; then                
        echo "mkfs.ext4 ${devname}p2 error."         
        return -1                                    
    fi 
    
    partname="data"
    sgdisk -g -n 3:1843235:`sgdisk -E ${devname}` -c 3:"${partname}" ${devname}
    ProcessExitCode=$?
    if [ ${ProcessExitCode} -ne 0 ]; then
        echo "create data error:${ProcessExitCode}"
        return $ProcessExitCode
    fi

    sleep 3                                                                                
    if [ ! -e ${devname}p3 ];then                                                          
        echo "do not exist ${devname}p3"                                               
        return -1                                                                      
    fi      

    mkfs.ext4 ${devname}p3
    ProcessExitCode=$?                                            
    if [ ${ProcessExitCode} -ne 0 ]; then                
        echo "mkfs.ext4 ${devname}p3 error."         
        return -1                                    
    fi 

    sleep 1 

    return 0
}


function getEMMcBlkIndex
{
    local iIndex
    local strMmcPath
    local strLinkPath
    for (( iIndex=0;iIndex<32;iIndex++ ))
    do
        strMmcPath=
        strLinkPath="/sys/block/mmcblk${iIndex}"
        if [ -e "${strLinkPath}" ];then
          strMmcPath=`readlink ${strLinkPath}|grep mmc_host/mmc1`
          if [ "x${strMmcPath}" != "x" ];then
              return ${iIndex}
          fi
        fi
    done
    return 255
}

function mountEMMC 
{
    local srcpath=$1
    local distpath=$2
    local devnum=-1
    local partnum=0
    local curdev=""
    if [ ! -d ${distpath} ];then
        mkdir ${distpath}
    fi

    mount | grep "$distpath" &>/dev/null                   
        if [ $? -eq 0 ];then 
        return 0                                   
        fi

    getEMMcBlkIndex
    devnum=$?
    if [ "${devnum}" == "255" ];then
        return -1
    fi

    curdev=/dev/mmcblk${devnum}
    if [ ! -e ${curdev} ]; then
        return -1
    fi

    partnum=`sgdisk -p ${curdev} | grep -n "${srcpath}" | awk -F ' ' '{ print $2 }'`
    if [ -z ${partnum} ];then
        partnum=0
    fi
        
    if [ ${partnum} -eq 0 ];then
        createEMMCPART  ${curdev}
        ProcessExitCode=$?                                                                                                                      
                if [ ${ProcessExitCode} -ne 0 ]; then                                                                                                   
                        echo "create emmc partition err:${ProcessExitCode}"
            return -1                                                                              
                fi
        partnum=`sgdisk -p ${curdev} | grep -n "${srcpath}" | awk -F ' ' '{ print $2 }'`
        if [ ${partnum} -eq 0 ];then
            return -1
        fi
    fi
    
    curdevpart=${curdev}p${partnum}
    FsType=`blkid $curdevpart | awk -F ' ' '{ print $4 }' | awk -F '=' '{ print $2 }' | tr -d "\""`
    if [ "${FsType}" = "ext3" ]; then
                MountOptions="noatime,data=journal"
        else
                MountOptions=""
        fi

    if [ ! -z "${MountOptions}" ]; then
                MountOptCmd="-o "${MountOptions}" -t ext4"
        else
                MountOptCmd=" -t ext4"
        fi

    echo "mount ${MountOptCmd} ${curdevpart} $distpath"
    mount ${MountOptCmd} ${curdevpart} $distpath  > /dev/null 2>&1 
    ProcessExitCode=$?                                                                     
        if [ ${ProcessExitCode} -ne 0 ]; then                                           
            echo "mount ${MountOptCmd} ${curdevpart} $distpath error."             
                return -1                                                       
        fi
   
    return 0
}

function umountEMMC
{
    #"rootfs" "/mnt/rootfs"
    local distpath=$1
    mount | grep "$distpath" &>/dev/null                                                   
    if [ $? -eq 0 ];then 
        umount ${distpath}                                                                   
    fi
}


function deleteEMMcPartition
{
    local iBlkIndex                             
    local iRetValue                                              
    local strDiskPath
    iBlkIndex=$1                                                 
    if [ "x${iBlkIndex}" == "x" ];then                           
        echo "Empty EMMc index"                                  
        return -1                                                
    fi   
    strDiskPath="/dev/mmcblk${iBlkIndex}"
    if [ ! -e "${strDiskPath}" ];then
        echo "disk ${strDiskPath} not exist"
        return -1
    fi

fdisk "${strDiskPath}" <<EOF
d
1
d
2
d
3
d
4
d
5
d
6
d
7
d
8
d
9
d



w



EOF

    iRetValue=$?
    return ${iRetValue}
}


function do_emmc_rootfs()
{
    local image=$1
    local umconfig=0
    local umdata=0
    local devnum=-1 
    local iRootfsDev=0
    local iConfigDev=0
    local iDataDev=0
    
    
    UpgradeClient "Upgrade ${image}......"

        getEMMcBlkIndex                                                                                                                   
        devnum=$?                                                                                                                         
        if [ "${devnum}" == "255" ];then
            UpgradeClient "Failed:Get EMMC device failed"                                                           
            return -1                                                                         
        fi                                                                                                            
                                                                                                                      
        curdev="/dev/mmcblk${devnum}"                                                                                                                
        if [ ! -e ${curdev} ]; then
            UpgradeClient "Failed:device %{curdev} not exist"                                                                                                                 
            return -1                                                                                                                                
        fi                                                                                                                                               
                                                                                                                                                         
        partnum=`sgdisk -p ${curdev} | grep -n "rootfs" | awk -F ' ' '{ print $2 }'` 
        if [ -z ${partnum} ];then                                                                                                         
                partnum=0                                                                                                                 
        fi
        iRootfsDev=`sgdisk -p ${curdev} | grep -n "rootfs" | awk -F ' ' '{ print $2 }'`
        iConfigDev=`sgdisk -p ${curdev} | grep -n "config" | awk -F ' ' '{ print $2 }'`
        iDataDev=`sgdisk -p ${curdev} | grep -n "data" | awk -F ' ' '{ print $2 }'`
        
        partnum="${iRootfsDev}"
        if [ "x${iRootfsDev}" == "x" -o "x${iConfigDev}" == "x"  -o  "x${iDataDev}" == "x" ];then

                deleteEMMcPartition ${devnum}                                                                                                                 
                createEMMCPART ${curdev}                                                    
                ProcessExitCode=$?                                                                
                if [ ${ProcessExitCode} -ne 0 ]; then                                                                                     
                        echo "Failed: create emmc partition err:${ProcessExitCode}"   
                        UpgradeClient "Failed:create emmc partition err:${ProcessExitCode}"                                                             
                        return -1                                                                                                         
                fi                                                                                                                        
        fi


    mountEMMC "rootfs" "/mnt/rootfs"
    ProcessExitCode=$?
    if [ ${ProcessExitCode} -ne 0 ]; then
    	return -1
    fi

    ROOTFS_DIR=/mnt/rootfs
    cd ${ROOTFS_DIR}
    deletfile "${ROOTFS_DIR}/mnt"
    deletfile "${ROOTFS_DIR}/LocalMedia"
    deletfile "${ROOTFS_DIR}/Config"
    deletfile "${ROOTFS_DIR}/data"
    DeleteOldFiles "${ROOTFS_DIR}"
    BackupCurSystemSettings "${ROOTFS_DIR}"
    UpgradeClient "Start extracting file ${image} ..."
    zcat ${image} | tar xpf -
    res=$?
    if [ $res -ne 0 ]; then
    	UpgradeClient "ERROR: extracting rootfs file failed."
        sleep 3
        return -1
    else
        UpgradeClient "$image is successfully upgraded."
    fi
    
    if [ -f ${UpgradeFilesSrcDir}/${Mp7xxDefaultConfigFile} ]; then
        mountEMMC "config" "/mnt/config"
        ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "mount config err ${ProcessExitCode}"
                fi

                if [ ! -d /mnt/config/Config ]; then
                        mkdir /mnt/config/Config
                fi

                if [ -h ${ROOTFS_DIR}/MP7XX/Config ];then
                        mv ${ROOTFS_DIR}/MP7XX/Config ${ROOTFS_DIR}/MP7XX/Config.bak
                        ln -s /mnt/config/Config ${ROOTFS_DIR}/MP7XX/Config
                fi
                umconfig=1

                DirToCntFileNum=${ROOTFS_DIR}/MP7XX/Config
                if [ -h ${DirToCntFileNum} ]; then
                        FileNumCnt=`ls ${DirToCntFileNum} | wc -l`
                        ProcessExitCode=$?
                        if [ ${ProcessExitCode} -ne 0 ]; then
                                echo "GetFileNumCnt(${DirToCntFileNum}) err ${ProcessExitCode}"                                                          
                        fi
                else
                        FileNumCnt=0
                fi

        if [ ${FileNumCnt} -eq 0 ]; then
                        UpgradeClient "Unpacking ${Mp7xxDefaultConfigFile}"
                        zcat ${UpgradeFilesSrcDir}/${Mp7xxDefaultConfigFile} | tar -xf -
                        ProcessExitCode=$?
                        if [ ${ProcessExitCode} -ne 0 ]; then
                                echo "UnpackFile(${Mp7xxDefaultConfigFile}) err ${ProcessExitCode}"
                        fi
                fi

                if [ -h ${ROOTFS_DIR}/MP7XX/Config.bak ];then
                        rm -rf ${ROOTFS_DIR}/MP7XX/Config
                        mv ${ROOTFS_DIR}/MP7XX/Config.bak ${ROOTFS_DIR}/MP7XX/Config
                fi
        fi


    if [ -f ${UpgradeFilesSrcDir}/${Mp7xxDefaultLocalMediaFile} ]; then
                mountEMMC "data" "/mnt/data"
        	ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "mount data err ${ProcessExitCode}"
                fi
                umdata=1

                if [ ! -d /mnt/data/LocalMedia ];then
                        mkdir /mnt/data/LocalMedia
                fi

		if [ ! -d /mnt/data/data ];then
        		mkdir /mnt/data/data
    		fi

		if [ -f ${ROOTFS_DIR}/UserData/data/AppInfo.ini ];then
                        cp ${ROOTFS_DIR}/UserData/data/AppInfo.ini /mnt/data/data/AppInfo.ini -rf
                fi

                if [ -h ${ROOTFS_DIR}/MP7XX/LocalMedia ];then
                        mv ${ROOTFS_DIR}/MP7XX/LocalMedia ${ROOTFS_DIR}/MP7XX/LocalMedia.bak
                        ln -s /mnt/data/LocalMedia ${ROOTFS_DIR}/MP7XX/LocalMedia
                fi

                DirToCntFileNum=${ROOTFS_DIR}/MP7XX/LocalMedia/                                                                                           
                if [ -h ${DirToCntFileNum} ]; then
                        FileNumCnt=`find ${DirToCntFileNum} -type f | wc -l`
                        ProcessExitCode=$?
                        if [ ${ProcessExitCode} -ne 0 ]; then
                                echo "GetFileNumCnt(${DirToCntFileNum}) err ${ProcessExitCode}"                                                     
                        fi
                else
                        FileNumCnt=0
                fi

                if [ ${FileNumCnt} -eq 0 ]; then
                    UpgradeClient "Unpacking ${Mp7xxDefaultLocalMediaFile}"
                    zcat ${UpgradeFilesSrcDir}/${Mp7xxDefaultLocalMediaFile} | tar -xf -
                    ProcessExitCode=$?
                    if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "UnpackFile(${Mp7xxDefaultLocalMediaFile}) err ${ProcessExitCode}"
                    fi
                fi
    
        if [ -h ${ROOTFS_DIR}/MP7XX/LocalMedia.bak ];then
                        rm -rf ${ROOTFS_DIR}/MP7XX/LocalMedia
                        mv ${ROOTFS_DIR}/MP7XX/LocalMedia.bak ${ROOTFS_DIR}/MP7XX/LocalMedia
                fi
        fi

    ResotoreCurSystemSettings
    DeleteTempBackupFiles
    cd ${TOPDIR}

    echo "" > $RootfsDir/update_is_successful
    DeleteTempBackupFiles
    deletfile "${ROOTFS_DIR}/Temp"
    rm -fr $UpgradeFilesSrcDir
    rm -f $RootfsDir/update_is_successful

    sync
    sleep 1

    	if [ ${umconfig} -eq 1 ]; then
                umountEMMC "/mnt/config"
                ProcessExitCode=$?
                if [ ${ProcessExitCode} -ne 0 ]; then
                        echo "umount config err ${ProcessExitCode}"                                                                                     
                fi
        fi

        if [ ${umdata} -eq 1 ]; then
                umountEMMC "/mnt/data"
                ProcessExitCode=$?                                                                                                                        
                if [ ${ProcessExitCode} -ne 0 ]; then                                                                                                     
                        echo "umount data err ${ProcessExitCode}"                                                                                                
                fi
        fi

        #umountEMMC "/mnt/rootfs"
        #ProcessExitCode=$?
        #if [ ${ProcessExitCode} -ne 0 ]; then
        #      echo "umount rootfs err ${ProcessExitCode}"
        #fi

        return 0

}


do_upgrade()
{
    local i tries res
    local imageFile imageType imageDir

    for i in 0 1 2; do
	eval imageType=${imagesName[$i]}
	eval imageFile=\$${imagesName[$i]}
	UpgradeClient "Upgrade $imageType: $imageFile"
	
	if [ $i -ne 2 ]; then
            imageDir=$UpgradeFilesTempDir
            if [ ! -f "$imageDir/$imageFile" ]; then
                echo -e "No image exists, skip.\n"
                continue
            fi
        else
            imageDir=$UpgradeFilesSrcDir # rootfs
        fi	

    	case $imageType in
    	    Zboot )
            do_zboot
            if [ $? -ne 0 ]; then
                UpgradeClient "ERROR: Zboot upgrade failed." 
               	return -1
            fi
	    
	    UpgradeClient "Upgrade Zboot Successfully." 
            ;;

            Kernel )
	    do_kernel $imageDir/${imageFile}                                           
            if [ $? -ne 0 ]; then                                              
                UpgradeClient "ERROR: Linux kernel upgrade failed."                                                                                                 	 return -1
	    fi
	    
	    UpgradeClient "Upgrade Kernel Successfully."
            ;;

            Rootfs )
	    do_emmc_rootfs $imageDir/${imageFile}
	    if [ $? -ne 0 ]; then                                                                                                                                        UpgradeClient "ERROR: rootfs upgrade failed."
            	return -1
	    fi 
	
	    UpgradeClient "Upgrade rootfs Successfully."
	    ;;
    	esac
        echo
    done

    return 0	
}

function DeleteOldFiles()
{
	local BaseDir=$1
	local iProcessExitCode=0
	local FileToDel=""

	while read OneLine
	do
		OneLine=${OneLine//$'\r'/}
		OneLine=${OneLine//$'\n'/}
		#echo OneLine=${OneLine}
		if [ ! -z "${OneLine}" ]; then
			FileToDel="${BaseDir}${OneLine}"
			if [ -e "${FileToDel}" ]; then
				rm -fr "${FileToDel}"
				iProcessExitCode=$?
				if [ ${iProcessExitCode} -ne 0 ]; then
					echo "Del(${FileToDel}) err ${iProcessExitCode}"
				fi
			fi
		fi
	done < "${UpgradeFilesSrcDir}/FilesToDelete.txt"
}

function BackupCurSystemSettings()
{
	local NAND_ROOTFS_BASE_DIR=$1
	local iOutRet=0
	local iProcessExitCode=0
	local iFileCntToProcess=0
	
	while [ 1 ]
	do
		if [ ! -e "${UpgradeFilesSrcDir}/FilesToBackup.txt" ]; then
			break;
		fi
			
		if [ ! -d "${SystemSettingsBackupDir}" ]; then
			mkdir -p "${SystemSettingsBackupDir}"
			iProcessExitCode=$?
			if [ ${iProcessExitCode} -ne 0 ]; then
				echo "MkDir(${SystemSettingsBackupDir}) err ${iProcessExitCode}"
				break;
			fi
		fi
	
		rm -fr "${CorrectFileListToBackupFile}"
		iProcessExitCode=$?
		if [ ${iProcessExitCode} -ne 0 ]; then
			echo "DelFile(${CorrectFileListToBackupFile}) err ${iProcessExitCode}"
			break;
		fi
		
		echo -n > "${CorrectFileListToBackupFile}"
		iProcessExitCode=$?
		if [ ${iProcessExitCode} -ne 0 ]; then
			echo "CreateFile(${CorrectFileListToBackupFile}) err ${iProcessExitCode}"
			break;
		fi
		
		iFileCntToProcess=0
		while read OneLine
		do
			OneLine=${OneLine//$'\r'/}
			OneLine=${OneLine//$'\n'/}
			#echo OneLine=${OneLine}
			TargetRootfsFile="${NAND_ROOTFS_BASE_DIR}${OneLine}"
			if [ -e "${TargetRootfsFile}" ]; then
				echo ${TargetRootfsFile}
				echo ${TargetRootfsFile} >> "${CorrectFileListToBackupFile}"
				iProcessExitCode=$?
				if [ ${iProcessExitCode} -ne 0 ]; then
					echo "Write(${CorrectFileListToBackupFile}) err ${iProcessExitCode}"
					break;
				fi
				((iFileCntToProcess++))
			fi
		done < "${UpgradeFilesSrcDir}/FilesToBackup.txt"
	
		if [ ${iFileCntToProcess} -gt 0 ]; then
			cat ${CorrectFileListToBackupFile} | xargs tar -cf ${SysSetBackupPackageFile}
			iProcessExitCode=$?
			if [ ${iProcessExitCode} -ne 0 ]; then
				echo "Tar(${CorrectFileListToBackupFile}) err ${iProcessExitCode}"
				break;
			fi
		fi

		break;
	done
	
	return ${iOutRet}
}

function ResotoreCurSystemSettings()
{
	while [ 1 ];
	do
		if [ ! -e "${CorrectFileListToBackupFile}" -o ! -e "${SysSetBackupPackageFile}" ]; then
			#echo "Not exist!"
			break;
		fi
		
		# Delete them first
		while read OneLine
		do
			OneLine=${OneLine//$'\r'/}
			OneLine=${OneLine//$'\n'/}
			#echo OneLine=${OneLine}
			rm -fr "${OneLine}"
			iProcessExitCode=$?
			if [ ${iProcessExitCode} -ne 0 ]; then
				echo "Del(${OneLine}) err ${iProcessExitCode}"
				break;
			fi
		done < "${CorrectFileListToBackupFile}"
		
		cd /
		iProcessExitCode=$?
		if [ ${iProcessExitCode} -ne 0 ]; then
			echo "ChangeDir(/) err ${iProcessExitCode}"
			break;
		fi
		tar -xf "${SysSetBackupPackageFile}" &> /dev/null
		iProcessExitCode=$?
		if [ ${iProcessExitCode} -ne 0 ]; then
			echo "Extract(SysSetBackupPackageFile) err ${iProcessExitCode}"
			break;
		fi
		
		break;
	done
}

function DeleteTempBackupFiles()
{
	local iOutRet=0
	local iProcessExitCode=0

	rm -fr "${SystemSettingsBackupDir}"
	iProcessExitCode=$?
	if [ ${iProcessExitCode} -ne 0 ]; then
		echo "Del(${SystemSettingsBackupDir}) err ${iProcessExitCode}"
	fi
	
	return ${iOutRet}
}

echo "-------------------------------"
echo "    Generic upgrading script   "
echo "-------------------------------"
echo -e "   Press 'Q' key to exit \n"
if read -t1 -n1 -s char; then
    if [ $char = "q" ] || [ $char = "Q" ]; then
        return
    fi
fi

do_checkchipid
if [ $? -ne 0 ]; then
    ExitProcess
fi

do_upgrade
if [ $? -ne 0 ]; then
	 UpgradeClient "Update failed." 
else
	 UpgradeClient "Upgrade successfully." 
fi




