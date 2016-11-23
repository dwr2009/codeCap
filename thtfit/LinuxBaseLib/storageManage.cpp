
#include <sys/mount.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <storageManage.h>
#include <BaseErrDef.h>
#include <memory.h>
#include <ErrPrintHelper.h>
#include <stdlib.h>
#include <StackBufString.h>
#include <string_ex.h>
#include <DirectoryManage.h>

StorageManage::StorageManage()
{
	m_NumPart = 0;
	ZeroMemory(m_DiscNodeMap,sizeof(m_DiscNodeMap));
}

StorageManage::~StorageManage()
{
	m_NumPart = 0;
	ZeroMemory(m_DiscNodeMap,sizeof(m_DiscNodeMap));
}

INT_t StorageManage::GetDevType(LPCSTR partName,pDevType pType)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	char orgPath[1024];
	char dirBuf[MAX_LINE_LEN];
	
	do{
		if(NULL == partName || NULL == pType)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		ZeroMemory(dirBuf,sizeof(dirBuf));
		if(strstr(partName,"sd"))
		{
			strcat(dirBuf,DEVICE_PATH);
			strcat(dirBuf,partName);
			iRet = readlink(dirBuf,orgPath,1024-1);
			if(iRet < 0)
			{
				iOutRet = ERROR_CALL_SYSTEM_FAILED;
				break;
			}
			
			if(strstr(orgPath,"/usb"))
			{
				*pType = USB_DEVICE;
			}
			else
			{
				*pType = HDD_DEVICE;
			}
		}
		else if(strstr(partName,"mmc"))
		{
			*pType = SDCARD_DEVICE;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t StorageManage::ParsePartitionsFile()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	FILE *fd = NULL;
	char Line[MAX_LINE_LEN];
	char ptName[MAX_NAME_LEN];
	char *Start = NULL;
	INT_t Index = 0;
	
	do{
		fd = fopen(PARTITION_PATH,"r");
		if(NULL == fd)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		while(fgets(Line, sizeof(Line),fd))
		{
			if(Index >= MAX_DISC_NUM)
        		{
        			iOutRet = ERROR_INVALID_PARAMETER;
            			break;
        		}
			
			ZeroMemory(ptName,sizeof(ptName));
			
			if (sscanf(Line, " %u %u %u %[^\n ]",&m_NodePartInfo[Index].major, &m_NodePartInfo[Index].minor,
				&m_NodePartInfo[Index].size, ptName) != 4)
			{
				continue;
			}

			iRet = GetDevType(ptName,&m_NodePartInfo[Index].EDevType);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				goto	OUT_ERROR;
			}
			
			for(Start = ptName; *Start; Start++)
			{
				continue;
			}
//			LOG_BLINE(" %s dev %d\n",ptName,m_NodePartInfo[Index].EDevType);
			/* note: excluding '0': e.g. mmcblk0 is not a partition name! */
			if (Start[-1] >= '1' && Start[-1] <= '9')
			{
			}
			else
			{
				m_NodePartInfo[Index].isRootNode = TRUE;
			}
			
			if(1 == m_NodePartInfo[Index].size)
			{
				m_NodePartInfo[Index].isExtPartition = TRUE;
			}
			
			strncpy(m_NodePartInfo[Index].NodePartName,ptName,strlen(ptName));
			Index ++;
			ZeroMemory(&Line,sizeof(Line));
		}
		
		if(0 == Index)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}

		m_NumPart = Index;
		
	}while(FALSE);

	if(NULL != fd)
	{
		fclose(fd);
		fd = NULL;
	}
	
OUT_ERROR:

	return iOutRet;
}

/* buf looks like ://GET FS TYPE
/dev/sdb2 /mnt/sdb2 vfat rw,nodiratime,fmask=0022,dmask=0022,codepage=cp437,iocharset=cp936 0 0
*/
DiscFsType StorageManage::ParseFStypeOnMtabLine(LPCSTR buf)
{    
	if(buf == NULL)
	{
		return FS_UNKNOWN ;
	}
	
	if(strstr(buf,"vfat"))
	{
		return  FS_VFAT;
	}
	else
	if(strstr((LPCSTR)buf,"ntfs") || strstr((LPCSTR)buf,"fuseblk"))
	{
		return FS_NTFS;
	}
	else
	if(strstr((LPCSTR)buf,"ext2"))
	{
		return FS_EXT2;
	}
	else
	if(strstr((LPCSTR)buf,"ext3"))
	{
		return FS_EXT3;
	}
	else
	if(strstr((LPCSTR)buf,"romfs"))
	{
		return FS_ROM;
	}
	else
	{        
		return FS_UNKNOWN;
	}
}

INT_t StorageManage::GetMapInfo(DNodeMap *pDNodeMapInfo,LPCSTR buf)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT_t i = 0, j = 0,space = 0;
	BOOL_t   isMNode = FALSE;
	LPSTR pDevNode = NULL;
	LPSTR pMouNode = NULL;

	do{
		if(buf == NULL || pDNodeMapInfo == NULL)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

    		pDNodeMapInfo->eFsType = ParseFStypeOnMtabLine(buf);
    		if(FS_UNKNOWN == pDNodeMapInfo->eFsType)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
    
    		pDevNode = pDNodeMapInfo->devNode;
		pMouNode = pDNodeMapInfo->mountNode;
    
    		while(buf[i] != '\0')
		{
			if(buf[i] == ' ')
			{
				pDevNode[i] = '\0';
				isMNode = TRUE;
				i ++;
				space ++;
				if(space > 1){break;}
				else {continue;}		
			}

			if(isMNode == FALSE)
			{
				pDevNode[i] = buf[i];
			}
			else
			{
				pMouNode[j] = buf[i];
				j ++ ;
			}
			i ++;
		}
			
    		pMouNode[j] = '\0';
		if(!access(pDevNode,F_OK))
		{
			pDNodeMapInfo->isExist = TRUE;        
		}
		else
		{
			pDNodeMapInfo->isExist = FALSE;
		}
	}while(FALSE);

    return iOutRet;
}

INT_t StorageManage::ParseMtabFile()
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
    	FILE* fd = NULL;
	char tmpBuf[256];
	UINT_t Index = 0;

	do{
		ZeroMemory(m_DiscNodeMap,sizeof(m_DiscNodeMap));
		fd = fopen(ETC_MTAB,"r");
		if(fd == NULL)
		{
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
		
		fseek(fd,0,SEEK_SET);
		
		while(fgets(tmpBuf,256,fd))
    		{
        		if(Index >= MAX_DISC_NUM)
        		{
        			iOutRet = ERROR_INVALID_PARAMETER;
            			break;
        		}
        
        		if(FS_UNKNOWN != ParseFStypeOnMtabLine(tmpBuf))         
        		{
				iRet = GetMapInfo(&m_DiscNodeMap[Index],tmpBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet; 
				}
				
				if(TRUE == m_DiscNodeMap[Index].isExist)
				{
					Index++;
				}
			}
		}

		if(0 == Index)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}

		m_NodePartMountNum = Index;
	}while(FALSE);

	if(NULL != fd)
	{
		fclose(fd);
		fd = NULL;
	}
	return iOutRet;
}

INT_t StorageManage::InitStorageMedia(void)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	do{
		ZeroMemory(m_NodePartInfo,sizeof(m_NodePartInfo));
		m_NumPart = 0;
		
		iRet = ParsePartitionsFile();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
				
		if(0 == m_NumPart)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}

		iRet = ParseMtabFile();
		if(ERROR_NOT_FOUND == iRet)
		{
			LOG_BLINE("WARNING : Not Found Mounted Info\n");
		}
		else
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

BOOL_t StorageManage::FindDevType(UINT_t mediaType)
{
	UINT_t Index;

	for(Index = 0;Index < m_NumPart;Index ++)
	{
		if( (typeof(DevType))mediaType == m_NodePartInfo[Index].EDevType)
		{
			return TRUE;
		}
	}
	return FALSE;
}

INT_t StorageManage::GetDevRootNode(UINT_t mediaType,char *pDevNode,UINT_t DevNodeSize)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT_t Index;
	BOOL_t isFound = FALSE;

	do{
		if(NULL == pDevNode || 0 == DevNodeSize)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		for(Index = 0;Index < m_NumPart;Index ++)
		{
			if((typeof(DevType))mediaType == m_NodePartInfo[Index].EDevType
				&& TRUE == m_NodePartInfo[Index].isRootNode)
			{
				if(strlen(m_NodePartInfo[Index].NodePartName) > DevNodeSize)
				{
					iOutRet = ERROR_NOT_ENOUGH_BUFFER;
					goto OUT_ERROR;
				}
				
				strncpy(pDevNode,m_NodePartInfo[Index].NodePartName,strlen(m_NodePartInfo[Index].NodePartName));
				isFound = TRUE;
				break;
			}
		}

		if(FALSE == isFound)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		
	}while(FALSE);
	
OUT_ERROR:
	return iOutRet;
}

INT_t StorageManage::GetPartNum(UINT_t mediaType,UINT_t *pPartCnt)
{
	INT_t iOutRet = ERROR_SUCCESS;
	UINT_t Index;
	UINT_t count = 0;
	
	do{
		if(NULL == pPartCnt)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		for(Index = 0;Index < m_NumPart;Index ++)
		{
			if((typeof(DevType))mediaType == m_NodePartInfo[Index].EDevType
				&& FALSE == m_NodePartInfo[Index].isRootNode)
			{
				count ++;
			}
		}
		
		*pPartCnt = count;
	}while(FALSE);
	
	return iOutRet;
}

INT_t StorageManage::IsMountedPart(LPCSTR pPartName,BOOL_t *pFound,UINT_t *pIndex)
{
	INT_t iOutRet = ERROR_SUCCESS,iRet;
	UINT_t Index;
	BOOL_t bFound = FALSE;

	do{
		if(NULL == pPartName ||NULL == pFound || NULL == pIndex)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		for(Index = 0; Index < m_NodePartMountNum; Index ++)
		{
			LOG_BLINE("[%s:%d]pPartName=%s,sName=%s\n",__func__,__LINE__,pPartName,(const char *)m_DiscNodeMap[Index].mountNode);
			iRet = strcmp_safe(pPartName,(const char *)m_DiscNodeMap[Index].mountNode);
			if(ERROR_SUCCESS == iRet)
			{
				bFound = TRUE;
				break;
			}
		}

		if(FALSE ==bFound)
		{
			iOutRet = ERROR_NOT_FOUND;
		}
		else
		{
			*pFound  = TRUE;
			*pIndex = Index;
		}
		
	}while(FALSE);

	return iOutRet;
}

INT_t StorageManage::LinkHDD(LPCSTR pPartName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	char devNodeBuf[MAX_NAME_LEN] = {0};
	char mntNodeBuf[MAX_NAME_LEN] = {0};
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
	
	do{
		if(NULL == pPartName)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		strcat(devNodeBuf,DEVNODE_PATH);
		strcat(devNodeBuf,pPartName);

		strcat(mntNodeBuf,MOUNTNODE_PATH);
		strcat(mntNodeBuf,pPartName);

		//LOG_BLINE("devNodeBuf : %s mntNodeBuf : %s\n",devNodeBuf,mntNodeBuf);
		iRet = CreateDirectory((LPCSTR)mntNodeBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = mount(devNodeBuf,mntNodeBuf,"ext3",0,NULL);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = strShellCmd.Format("%s Mount %s",MEDIA_MNT_PATH,mntNodeBuf);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		if(0 == access(HDD_NODE_PATH,F_OK))
		{
			unlink(HDD_NODE_PATH);
		}

		iRet = system(strShellCmd);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t StorageManage::OprationEachPart(EOprateTypeMedia option,UINT_t mediaType,LPCSTR pPartName)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t bMounted = FALSE;
	UINT_t index = 0;
	char devNodeBuf[MAX_NAME_LEN] = {0};
	char mntNodeBuf[MAX_NAME_LEN] = {0};

	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);

	do{
		if(NULL == pPartName)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		strcat(devNodeBuf,DEVNODE_PATH);
		strcat(devNodeBuf,pPartName);

		strcat(mntNodeBuf,MOUNTNODE_PATH);
		if(USB_DEVICE == mediaType)
		{
			strcat(mntNodeBuf,"usb_");
			strcat(mntNodeBuf,pPartName);
		}
		else
		{
			strcat(mntNodeBuf,pPartName);
		}
		//LOG_BLINE("pPartName : %s\n",pPartName);
		iRet = IsMountedPart(mntNodeBuf,&bMounted,&index);
		if(ERROR_SUCCESS == iRet && TRUE == bMounted)//is mounted
		{
			//LOG_BLINE("%s:%d umount media\n",__func__,__LINE__);
			iRet = umount(mntNodeBuf);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = ERR_SYSTEM_CALL_UMOUNT_FAIL;
				break;
			}
		}
			
		if(SDCARD_DEVICE == mediaType ||USB_DEVICE == mediaType)
		{
			if(FORMAT_OPTION_MEDIA == option)
			{
				if(strstr(pPartName,"mmcblk1") || USB_DEVICE == mediaType)
				{
					//LOG_BLINE("%s:%d mkdos for SDCard Device or USB Device\n",__func__,__LINE__);
					iRet = strShellCmd.Format("mkdosfs %s",devNodeBuf);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
				}
				else
				{
					LOG_BLINE("the system disk partion shouldn't be format\n");
					break;
				}
			}
			else if(REPAIR_OPTION_MEDIA == option)
			{
				iRet = strShellCmd.Format("dosfsck -V -r %s",devNodeBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			//LOG_BLINE("mount device which has been format bMounted = %d\n",bMounted);
			if(TRUE == bMounted)
			{
				//LOG_BLINE("devNode=%s,mountNode=%s\n",m_DiscNodeMap[index].devNode,m_DiscNodeMap[index].mountNode);
				iRet = mount(m_DiscNodeMap[index].devNode,m_DiscNodeMap[index].mountNode,"vfat",0,NULL);
				if(ERROR_SUCCESS != iRet)
				{
					LOG_BLINE("error to mount\n");
					iOutRet = iRet;
					break;
				}
			}
			else
			{
				iRet = CreateDirectory((LPCSTR)mntNodeBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				iRet = mount(devNodeBuf,mntNodeBuf,"vfat",0,NULL);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
		}
		else if(HDD_DEVICE == mediaType)
		{
			if(FORMAT_OPTION_MEDIA == option)
			{
				iRet = strShellCmd.Format("mkfs.ext3 %s",devNodeBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else if(REPAIR_OPTION_MEDIA == option)
			{
				iRet = strShellCmd.Format("fsck.ext3 -y %s",devNodeBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}

			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			if(TRUE == bMounted)
			{
				iRet = mount(m_DiscNodeMap[index].devNode,m_DiscNodeMap[index].mountNode,"ext3",0,NULL);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else
			{
				iRet = CreateDirectory((LPCSTR)mntNodeBuf);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}

				iRet = mount(devNodeBuf,mntNodeBuf,"ext3",0,NULL);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

INT_t StorageManage::OprateStorageMedia(UINT_t option,UINT_t mediaType)
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	BOOL_t isFoundDevType = FALSE;
	UINT_t partCnt = 0;
	char rootNode[MAX_NAME_LEN];
	DECLARE_CLS_STACK_BUF_STRING(strShellCmd, 1024);
	UINT_t Index;
	
	do{
		if( UNKNOW_DEVICES == (typeof(DevType))mediaType || RAM_DEVICE == (typeof(DevType))mediaType)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}

		if(END_OPTION_MEDIA == (typeof(EOprateTypeMedia))option)
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		
		//pre-judge the media type is existed
		isFoundDevType = FindDevType(mediaType);
		if(FALSE == isFoundDevType)
		{
			iOutRet = ERROR_NOT_FOUND;
			break;
		}
		//get root node
		ZeroMemory(rootNode,sizeof(rootNode));
		iRet = GetDevRootNode(mediaType,rootNode,sizeof(rootNode));
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}

		iRet = GetPartNum(mediaType,&partCnt);
		//Note!!!! No Partitions
		if(0 == partCnt)
		{
			if(USB_DEVICE == mediaType)
			{
				iRet = strShellCmd.Format("%s %d %s usb",FDISKSH_PATH,option,rootNode);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else if(SDCARD_DEVICE == mediaType)
			{
				iRet = strShellCmd.Format("%s %d %s sdcard",FDISKSH_PATH,option,rootNode);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			else if(HDD_DEVICE == mediaType)
			{
				iRet = strShellCmd.Format("%s %d %s hdd",FDISKSH_PATH,option,rootNode);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
			}
			
			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			if(HDD_DEVICE == mediaType)
			{
				strcat(rootNode,"1");
				//LOG_BLINE("rootNode : %s\n",rootNode);
				iRet = LinkHDD(rootNode);
				if(ERROR_SUCCESS != iRet)
				{
					iOutRet = iRet;
					break;
				}
				//LOG_BLINE("iRet : %d\n",iRet);
			}
			
			/*strcat(strBuf,MOUNTNODE_PATH);
			strcat(strBuf,rootNode);
			strcat(strBuf,"1");

			LOG_BLINE("strBuf : %s\n",strBuf);
			iRet = strShellCmd.Format("%s Mount %s",MEDIA_MNT_PATH,strBuf);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}

			iRet = system(strShellCmd);
			if(ERROR_SUCCESS != iRet)
			{
				iOutRet = iRet;
				break;
			}
			LOG_BLINE("iRet : %d\n",iRet)*/
		}
		else
		{
			for(Index = 0; Index < m_NumPart; Index++)
			{
				if((typeof(DevType))mediaType == m_NodePartInfo[Index].EDevType 
					&& FALSE == m_NodePartInfo[Index].isRootNode)
				{
					if(TRUE == m_NodePartInfo[Index].isExtPartition)
					{	
						LOG_BLINE("Found Extend Partition %s\n",m_NodePartInfo[Index].NodePartName);
						continue;
					}
					iRet = OprationEachPart((EOprateTypeMedia)option,mediaType,(LPCSTR)m_NodePartInfo[Index].NodePartName);
					if(ERROR_SUCCESS != iRet)
					{
						iOutRet = iRet;
						break;
					}
				}
			}
		}
	}while(FALSE);

	return iOutRet;
}

