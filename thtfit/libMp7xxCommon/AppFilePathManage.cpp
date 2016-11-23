#include "Api/AppFilePathManage.h"
#include <string.h>

#define	MP7XX_APP_BASE_DIR		"/MP7XX"

#ifdef	__cplusplus
namespace Mp7xxCommon
{
#endif	//__cplusplus

LPCSTR Mp7xx_getMountSymLinkFilePath(const Mp7xxCommon::MOUNT_DEV_TYPE eMountDevType)
{
	LPCSTR pszOutLinkFilePath = "";

	do
	{
		if(Mp7xxCommon::MOUNT_DEV_LocalMedia == eMountDevType)
		{
			pszOutLinkFilePath = (MP7XX_APP_BASE_DIR "/" LOCAL_STORAGE_DIR_NAME);
		}
		else if(Mp7xxCommon::MOUNT_DEV_USB == eMountDevType)
		{
			pszOutLinkFilePath = (MOUNTDIR MP7XX_APP_BASE_DIR "/" USB_STORAGE_DIR_NAME);
		}
		else if(Mp7xxCommon::MOUNT_DEV_SD == eMountDevType)
		{
			pszOutLinkFilePath = (MOUNTDIR MP7XX_APP_BASE_DIR "/" SD_CARD_DIR_NAME);
		}
		else if(Mp7xxCommon::MOUNT_DEV_HDD == eMountDevType)
		{
			pszOutLinkFilePath = (MOUNTDIR MP7XX_APP_BASE_DIR "/" StorageDirName_HDD);
		}
	}while(FALSE);

	return pszOutLinkFilePath;
}

Mp7xxCommon::MOUNT_DEV_TYPE Mp7xx_getMountDevType(LPCSTR pszMntSymLinkPath)
{
	Mp7xxCommon::MOUNT_DEV_TYPE eOutMntDevType = Mp7xxCommon::MOUNT_DEV_Uknown;

	do
	{
		if(NULL == pszMntSymLinkPath)
		{
			break;
		}
		else if('\0' == pszMntSymLinkPath[0])
		{
			break;
		}
		if(0 == strcmp((MP7XX_APP_BASE_DIR "/" LOCAL_STORAGE_DIR_NAME), pszMntSymLinkPath))
		{
			eOutMntDevType = Mp7xxCommon::MOUNT_DEV_LocalMedia;
		}
		else if(0 == strcmp((MOUNTDIR "/" MP7XX_APP_BASE_DIR "/" USB_STORAGE_DIR_NAME), pszMntSymLinkPath))
		{
			eOutMntDevType = Mp7xxCommon::MOUNT_DEV_USB;
		}
		else if(0 == strcmp((MOUNTDIR "/"MP7XX_APP_BASE_DIR "/" SD_CARD_DIR_NAME), pszMntSymLinkPath))
		{
			eOutMntDevType = Mp7xxCommon::MOUNT_DEV_SD;
		}
		else if(0 == strcmp((MOUNTDIR "/"MP7XX_APP_BASE_DIR "/" StorageDirName_HDD), pszMntSymLinkPath))
		{
			eOutMntDevType = Mp7xxCommon::MOUNT_DEV_HDD;
		}
	}while(FALSE);

	return eOutMntDevType;
}

#ifdef	__cplusplus
}
#endif	//__cplusplus

