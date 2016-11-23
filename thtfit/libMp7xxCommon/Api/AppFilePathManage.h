#ifndef	_APP_FILE_PATH_MANAGE_H_
#define	_APP_FILE_PATH_MANAGE_H_

#include <BaseTypeDef.h>

#ifdef	__cplusplus
namespace Mp7xxCommon
{
#endif	//__cplusplus

#define	LOCAL_STORAGE_DIR_NAME		"LocalMedia"
#define	SD_CARD_DIR_NAME			"SdMedia1P1"
#define	USB_STORAGE_DIR_NAME		"UsbMedia1P1"
#define	USB2_STORAGE_DIR_NAME		"UsbMedia1P2"//"UsbMedia2P1"
#define	StorageDirName_HDD			"Hdd"
#define	StorageDirName_RAM			"Ram"
#define MOUNTDIR					"/mnt"

typedef enum
{
	MOUNT_DEV_Uknown,
	MOUNT_DEV_LocalMedia = 1,
	MOUNT_DEV_USB = 2,
	MOUNT_DEV_SD = 3,
	MOUNT_DEV_HDD = 4,
}MOUNT_DEV_TYPE, *P_MOUNT_DEV_TYPE;

LPCSTR Mp7xx_getMountSymLinkFilePath(const Mp7xxCommon::MOUNT_DEV_TYPE eMountDevType);
Mp7xxCommon::MOUNT_DEV_TYPE Mp7xx_getMountDevType(LPCSTR pszMntSymLinkPath);


#ifdef	__cplusplus
}
#endif	//__cplusplus

#endif	//_APP_FILE_PATH_MANAGE_H_

