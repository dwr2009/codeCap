#!/bin/sh

PATH=/bin:/sbin:/usr/bin:/usr/sbin
#set -x

function killRuningProcess()
{
	UpgradeServer_Cmd=`ps -fA | grep "UpgradeServer" | grep -v "grep" | awk -F ' ' '{print $8}' | grep "UpgradeServer"`
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                UpgradeServer_Cmd=""
        fi

        if [ ! -z "${UpgradeServer_Cmd}" ]; then
                UpgradeServer_pid=`ps -fA | grep "${UpgradeServer_Cmd}" | grep -v "grep" | awk -F " " '{print $2}'`

                if [ ! -z "${UpgradeServer_pid}" ]; then
                        echo "nxserver is already running."
                        kill -9 ${UpgradeServer_pid}
                fi
        fi

	sleep 3

        nexus_Cmd=`lsmod | grep "nexus" | grep -v "grep" | awk -F " " '{print $1}'`
        ProcessExitCode=$?
        if [ ${ProcessExitCode} -ne 0 ]; then
                nexus_Cmd=""
        fi

        if [ ! -z "${nexus_Cmd}" ]; then
                rmmod nexus
        fi
	
	sleep 1

}

function Prepare()
{	

	killRuningProcess

	if [ -f /mnt/rootfs/SystemUpgrade/nexus.ko ]; then
		cp /mnt/rootfs/SystemUpgrade/nexus.ko /lib/ -rf
	fi

	if [ -f /mnt/rootfs/SystemUpgrade/libnexus.so ]; then
                cp /mnt/rootfs/SystemUpgrade/libnexus.so /lib/ -rf
        fi

	if [ -f /mnt/rootfs/SystemUpgrade/arial_18_aa.bwin_font ]; then
                cp /mnt/rootfs/SystemUpgrade/arial_18_aa.bwin_font /lib/ -rf
        fi

	if [ -f /mnt/rootfs/SystemUpgrade/UpgradeServer ]; then
                cp /mnt/rootfs/SystemUpgrade/UpgradeServer /bin/ -rf
        fi

	if [ -f /mnt/rootfs/SystemUpgrade/UpgradeClient ]; then
                cp /mnt/rootfs/SystemUpgrade/UpgradeClient /bin/ -rf
        fi	

	insmod /lib/nexus.ko
	sleep 3
	UpgradeServer&
	sleep 6
	
	UpgradeClient "Upgrade: preparing..."

}

do_checksum()
{
    echo "MD5 Checksum ..."
    UpgradeClient "Upgrade: do_checksum"

    cd /mnt/rootfs/SystemUpgrade
    md5sum -c md5sum
    if [ $? -ne 0 ]; then
        UpgradeClient "ERROR: MD5sum incorrect, must cancel the upgrade."
        return 1
    fi

    UpgradeClient "Checksum passed."
    return 0
}

if [ -d /mnt/rootfs/SystemUpgrade ]; then
    Prepare
    do_checksum
    if [ $? -ne 0 ]; then
        UpgradeClient "Warning: Removing the incorrect upgrade files!"
        cd /
        rm -fr /mnt/rootfs/SystemUpgrade
        sync
        exit 1
    fi

    # upgrade files
    cd /mnt/rootfs/SystemUpgrade
    mkdir -p /tmp/SystemUpgrade
    cp -rf bolt.bin chipid zImage Upgrade.sh /tmp/SystemUpgrade
    cd /

    umount /mnt/rootfs/
    sleep 1		
    UpgradeClient "Upgrade..."

    chmod +x /tmp/SystemUpgrade/Upgrade.sh
    sh /tmp/SystemUpgrade/Upgrade.sh
fi

killRuningProcess

