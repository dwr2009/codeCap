#include "ControlCenterApp.h"
#include <AppFilePathManage.h>
#include <BaseErrDef.h>
#include <DevConnectionStatus.h>
#include <StackBufString.h>
#include <SysBaseSrvApi.h>
#include <QProcess>
#include <BaseErrDef.h>
#include "WebBrowserApp.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

QControlCenterApp::QControlCenterApp(int & argc, char ** argv) : QApplication(argc, argv)
{
	m_bInternalActionOnGpioBtnEvt = TRUE;
}
QControlCenterApp::~QControlCenterApp()
{

}

int QControlCenterApp::On_FsMountPointEvent(ControlCenter::FS_MOUNT_EVENT_ID eFsMountEvtId,
	const QString & strFsMntSymFilePath)
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		if(strFsMntSymFilePath.isEmpty())
		{
			iOutRet = ERROR_INVALID_PARAMETER;
			break;
		}
		BOOL_t bMounted = (ControlCenter::FsMountEvt_SymLink == eFsMountEvtId);
		Mp7xxCommon::MOUNT_DEV_TYPE eMntDevType = Mp7xxCommon::Mp7xx_getMountDevType(strFsMntSymFilePath.toUtf8().constData());
		if(Mp7xxCommon::MOUNT_DEV_USB == eMntDevType)
		{
			m_Mp7xxGlobalStatusData.set_UsbIsMounted(bMounted);
		}
		else if(Mp7xxCommon::MOUNT_DEV_SD == eMntDevType)
		{
			m_Mp7xxGlobalStatusData.set_SdIsMounted(bMounted);
		}
		else if(Mp7xxCommon::MOUNT_DEV_HDD == eMntDevType)
		{
			m_Mp7xxGlobalStatusData.set_HddIsMounted(bMounted);
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QControlCenterApp::UpdateDevMountStatus()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	DECLARE_CLS_STACK_BUF_STRING(strPropVal, 32);

	do
	{
		//usb
		iRet = SysProp_get(SysProp_KEY_DevConn_UsbMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_UsbIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_UsbIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//sd
		iRet = SysProp_get(SysProp_KEY_DevConn_SdMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_SdIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_SdIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
		//Hdd
		iRet = SysProp_get(SysProp_KEY_DevConn_HddMounted, OUT strPropVal);
		if(ERROR_SUCCESS == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_HddIsMounted(strPropVal.toBool());
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else if(ERROR_NOT_FOUND == iRet)
		{
			iRet = m_Mp7xxGlobalStatusData.set_HddIsMounted(FALSE);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		else
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QControlCenterApp::RunShellScript(const QString & strShellScript, QString & strResult)
{
	INT_t iOutRet = ERROR_SUCCESS;
	bool bRet;
	QProcess oShellProcess;
	QStringList CmdParamList;
	strResult.clear();

	do
	{
		CmdParamList << "-c";
		CmdParamList << strShellScript;
		oShellProcess.start("sh", CmdParamList, QIODevice::ReadOnly);
		bRet = oShellProcess.waitForStarted(5*1000/*timeout ms*/);
		if(false == bRet)
		{
			iOutRet = ERROR_CREATE_PROCESS_FAILED;
			break;
		}
		if(QProcess::NotRunning != oShellProcess.state())
		{
			bRet = oShellProcess.waitForFinished();
		}
		//read data, if oShellProcess outputs too much data, it may be blocked due to not consume the data by any process.
		QByteArray StdOutData = oShellProcess.readAllStandardOutput();
		QString strIfConfigInfo(StdOutData);

		if(strShellScript.indexOf("ifconfig") != -1)
		{			
			strResult = QString("Mac addr 1:") + strIfConfigInfo.mid(strIfConfigInfo.indexOf("HWaddr ")+7,17)
				+QString("\n Mac addr 2:") + strIfConfigInfo.mid(strIfConfigInfo.lastIndexOf("HWaddr ")+7,17) ;

			 strResult +=  QString("\n\nIp addr 1: ") +  strIfConfigInfo.mid(strIfConfigInfo.indexOf("inet addr:")+10,(strIfConfigInfo.indexOf(" Bcast")- strIfConfigInfo.indexOf("inet addr:")-9));
			 strResult +=  QString("\nIp addr 2: ") +  strIfConfigInfo.mid(strIfConfigInfo.lastIndexOf("inet addr:")+10,(strIfConfigInfo.lastIndexOf(" Bcast")- strIfConfigInfo.lastIndexOf("inet addr:")-9));
		}
		else
		{
			strResult = StdOutData;
		}
	}while(FALSE);

	return iOutRet;
}


#define DEVICE_FILENAME  "/dev/Mp7xxGpioIRBlasterDevice"
#define IOCTL_GPIO_IR_BLASTER_SET _IOW( 'R', 0x01, unsigned char [2] )

int QControlCenterApp::CheckIRStatus(int usercode, int userkey)
{
	int retval;
#if 0
	int c;
    	int usercode = 0, userkey = 0;
	for (;;) 
	{
	        c = getopt(argc, argv, "u:k:?");
	        if (c < 0)
	            break;

	        if (strncmp(optarg, "0x", 2) == 0)
	            base = 16;
	        else 
	            base = 10;

	        switch (c) 
		{
	        case 'u':
	            usercode = strtol(optarg, NULL, base);
	            break;
	        case 'k':
	            userkey = strtol(optarg, NULL, base);
	            break;
	        case '?':
	          //  usage();
	            return 0;
	        default:
	       //     usage();
	            return -1;
	        }
	}
#endif	

    printf("GPIO IR Blasler: usercode=0x%02x, key=0x%02x\n", usercode, userkey);

    int dev = open(DEVICE_FILENAME, O_RDWR | O_NDELAY);

    if (dev >= 0) 
    	{
		unsigned char key[2];
		key[0] = (unsigned char)(usercode & 0xff);
		key[1] = (unsigned char)(userkey & 0xff);

		retval = ioctl(dev, IOCTL_GPIO_IR_BLASTER_SET, &key);
		if (retval != 0) {
		    perror("IOCTL_GPIO_IR_BLASTER_SET");
		}
		close(dev);
    	}
	else
	{
		return -1;
	}

	return 0;
}

int QControlCenterApp::setInternalActionOnGpioBtnEvt(const bool bEnable)
{
	int iOutRet = ERROR_SUCCESS;

	do
	{
		m_bInternalActionOnGpioBtnEvt = bEnable;
	}while(FALSE);

	return iOutRet;
}
#if 1/*by_su*/
int QControlCenterApp::On_ChangeLanguageDone(int data)
{
	return ERROR_SUCCESS;
}
#endif
