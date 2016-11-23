
#ifndef __STORAGEMANAGE_H__
#define __STORAGEMANAGE_H__

#include <BaseTypeDef.h>

#define ETC_MTAB        "/etc/mtab" 
#define DEVICE_PATH	    "/sys/class/block/"
#define PARTITION_PATH  "/proc/partitions"
#define FDISKSH_PATH		"/MP7XX/fdisk-storageMedia.sh"
#define HDDFSCK_PATH	"/MP7XX/fsck-hdd.sh"
#define MEDIA_MNT_PATH 	"/MP7XX/DevicePlugNotify.sh"
#define DEVNODE_PATH	"/dev/"
#define MOUNTNODE_PATH  "/mnt/"
#define HDD_NODE_PATH   "/MP7XX/HDD"
#define MAX_DISC_NUM    32
#define MAX_NAME_LEN  64
#define MAX_LINE_LEN	   256

typedef enum
{
	UNKNOW_DEVICES = -1,
	FLASH_DEVICE,
	USB_DEVICE,
	SDCARD_DEVICE,
	HDD_DEVICE,
	RAM_DEVICE,
	TUNER_DEVICE,
    	URL_DEVICE,
   	INVALID_DEVICE,
}DevType,*pDevType;

typedef enum{
	FS_UNKNOWN = 0,
	FS_VFAT,
	FS_NTFS,
	FS_EXT2,
	FS_EXT3,
	FS_JFFS,
	FS_ROM,
	FS_NFS,
	FS_SMBFS,
}DiscFsType;

typedef enum{
	FORMAT_OPTION_MEDIA,
	REPAIR_OPTION_MEDIA = 3,
	END_OPTION_MEDIA,
}EOprateTypeMedia;
typedef struct _TDNodeMap
{
	char devNode[MAX_NAME_LEN]; 
	char mountNode[MAX_NAME_LEN];
	DiscFsType eFsType;
	BOOL_t isExist;
}DNodeMap;

typedef struct {
	char NodePartName[MAX_NAME_LEN];
	INT_t major;
	INT_t minor;
	INT_t size;
	DevType EDevType;
	BOOL_t isRootNode;
	BOOL_t isMounted;
	BOOL_t isExtPartition;
}NodePartInfo,*pNodePartInfo;

class StorageManage
{
public:
	StorageManage();
	~StorageManage();
	
	INT_t InitStorageMedia(void);
	INT_t OprateStorageMedia(UINT_t option,UINT_t mediaType);
private:
	INT_t GetMapInfo(DNodeMap *pDNodeMapInfo,LPCSTR buf);
	DiscFsType  ParseFStypeOnMtabLine(LPCSTR buf);
	INT_t ParsePartitionsFile();
	INT_t ParseMtabFile();
	INT_t GetDevType(LPCSTR partName,pDevType pType);
	BOOL_t FindDevType(UINT_t mediaType);
	INT_t GetDevRootNode(UINT_t mediaType,char *pDevNode,UINT_t DevNodeSize);
	INT_t GetPartNum(UINT_t mediaType,UINT_t *pPartCnt);
	INT_t OprationEachPart(EOprateTypeMedia option,UINT_t mediaType,LPCSTR pPartName);
	INT_t IsMountedPart(LPCSTR pPartName,BOOL_t *pFound,UINT_t *pIndex);
	INT_t LinkHDD(LPCSTR pPartName);
private:
	UINT_t m_NodePartMountNum;
	DNodeMap  m_DiscNodeMap[MAX_DISC_NUM];
	UINT_t m_NumPart;
	NodePartInfo m_NodePartInfo[MAX_DISC_NUM];
};

#endif

