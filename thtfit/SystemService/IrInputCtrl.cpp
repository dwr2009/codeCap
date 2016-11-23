#include "IrInputCtrl.h"
#include <ErrPrintHelper.h>
#include <BaseErrDef.h>
#include "SystemServiceAppIf.h"
#include <QtWidgets/QApplication>
#include <QDBusError>
#include <QDebug>
#include <ProcessList.h>
#include <QStringList>
#include "SystemSrvApi.h"
#include <stdlib.h>

#include <sys/ioctl.h>
#include <fcntl.h>
#include "DbgLogSwitchDef.h"
#include <unistd.h>

#define DEVICE_FILENAME  "/dev/Mp7xxGpioIRBlasterDevice"
#define IOCTL_GPIO_IR_BLASTER_SET _IOW( 'R', 0x01, unsigned char [2] )

QDBusIf_IrInputCtrl::QDBusIf_IrInputCtrl(QIrInputCtrl * pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	m_pIrInputCtrl = pParentObj;	
	setAutoRelaySignals(true);
}

void QDBusIf_IrInputCtrl::EnableDisableInput(int bEnable, int & iOutRet)
{
	if(m_pIrInputCtrl)
	{
		m_pIrInputCtrl->EnableDisableInput(bEnable, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_IrInputCtrl::getInputEnabled(int & iOutRet, int & bEnabled)
{
	if(m_pIrInputCtrl)
	{
		m_pIrInputCtrl->getInputEnabled(iOutRet, bEnabled);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_IrInputCtrl::SendIrOut(int iIRCode, int & iOutRet)
{
	if(m_pIrInputCtrl)
	{
		m_pIrInputCtrl->SendIrOut(iIRCode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_IrInputCtrl::SendIrOut(unsigned int iIRCode, int & iOutRet)
{
	if(m_pIrInputCtrl)
	{
		m_pIrInputCtrl->SendIrOut(iIRCode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

void QDBusIf_IrInputCtrl::SendIrLevelOut(QString iIRLevelCode, int & iOutRet)
{
	if(m_pIrInputCtrl)
	{
		m_pIrInputCtrl->SendIrLevelOut(iIRLevelCode, iOutRet);
	}
	else
	{
		iOutRet = ERROR_INVALID_STATE;
	}
}

QIrInputCtrl::QIrInputCtrl()
{
	m_bIrInputEnabled = 1;
	m_pIrBlasterInstance=getIrBlasterInstance();
}

QIrInputCtrl::~QIrInputCtrl()
{
	m_DbusIf_IrInputCtrl_sp.clear();
}

INT_t QIrInputCtrl::InitInstance()
{
	INT_t iOutRet = ERROR_SUCCESS, iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	BOOL_t bRet;
	QSharedPointer <QSettings> SysSrvSettingsObj_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
		if(SysSrvSettingsObj_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}		

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		m_ProcessList_sp = QSharedPointer <CProcessList> (new CProcessList);
		if(m_ProcessList_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		m_DbusIf_IrInputCtrl_sp = QSharedPointer <QDBusIf_IrInputCtrl>(new QDBusIf_IrInputCtrl(this));
		if(m_DbusIf_IrInputCtrl_sp.isNull())
		{
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}
		bRet = DBusConn_sp->registerObject(DBUS_PATH_SysSrv_IrInputCtrl, this);
		if(false == bRet)
		{
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid())
			{
				LOG("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}
			else
			{
				LOG_BLINE("Failed to reg dbus obj %s\n", DBUS_PATH_SysSrv_IrInputCtrl);
			}
			break;
		}

		m_bIrInputEnabled = SysSrvSettingsObj_sp->value(KEY_UINPUT_INPUT_ENABLE, 1).toInt();
		BOOL_t bSaveSettings = FALSE;
		EnableDisableInput(m_bIrInputEnabled, iRet, bSaveSettings);
		if(ERROR_SUCCESS != iRet)
		{
			PRINT_BFILE_LINENO_IRET_STR;
		}
	}while(FALSE);

	return iOutRet;
}

INT_t QIrInputCtrl::ExitInstance()
{	
	INT_t iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf)
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull())
		{
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		if(DBusConn_sp->isConnected())
		{
			DBusConn_sp->unregisterObject(DBUS_PATH_SysSrv_IrInputCtrl); 		
		}

		m_ProcessList_sp.clear();
	}while(FALSE);
	
	return iOutRet;
}

void QIrInputCtrl::EnableDisableInput(int bEnable, int & iOutRet, bool bSaveSettings/* = TRUE*/)
{
	INT_t iRet;
	BOOL_t bUinputIsRunning = FALSE;

	iOutRet = ERROR_SUCCESS;

	do
	{
		iRet = m_ProcessList_sp->ScanSysProcess();
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
		iRet = CheckUinputRunning(OUT bUinputIsRunning);
		if(ERROR_SUCCESS != iRet)
		{
			iOutRet = iRet;
			break;
		}
#if 1/*added by lxj 2012-9-29 for irControl*/
		if(FALSE == bUinputIsRunning)
		{
			iRet = system(PROGRAM_NAME_UINPUT " &");
			if(0 != iRet)
			{
				iOutRet = ERROR_CREATE_PROCESS_FAILED;
				break;
			}
		}
		iRet = setIrControlEnabled(bEnable);/*for libGlobalStatus*/
		if(iRet != ERROR_SUCCESS){
			iOutRet = ERROR_INVALID_STATE;
			break;
		}
#else
		//LOG_BLINE("UinputIsRunning=%d\n", bUinputIsRunning);
		if(bEnable && bUinputIsRunning)
		{
			m_bIrInputEnabled = TRUE;
 		}
		if(FALSE == bEnable && FALSE == bUinputIsRunning)
		{
			m_bIrInputEnabled = FALSE;
 		}
		if(bEnable && FALSE == bUinputIsRunning)
		{
			iRet = system(PROGRAM_NAME_UINPUT " &");
			if(0 != iRet)
			{
				iOutRet = ERROR_CREATE_PROCESS_FAILED;
				break;
			}
		}
		if(FALSE == bEnable && bUinputIsRunning)
		{
			iRet = system("killall " PROGRAM_NAME_UINPUT);
			if(0 != iRet)
			{
				iOutRet = ERR_KILL_PROCESS_FAIL;
				break;
			}
		}
#endif
		if(bSaveSettings)
		{
			m_bIrInputEnabled = bEnable;
			//Save settings to the file
			{
				QSharedPointer <QSettings> SysSrvSettingsObj_sp;
				CSystemServiceAppIf * pSystemServiceAppIf = NULL;
				
				pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
				if(NULL == pSystemServiceAppIf)
				{
					iOutRet = ERROR_INVALID_STATE;
					break;
				}
				
				SysSrvSettingsObj_sp = pSystemServiceAppIf->getSysSrvSettingsObj();
				if(SysSrvSettingsObj_sp.isNull())
				{
					iOutRet = ERROR_INVALID_STATE;
					break;
				}		

				SysSrvSettingsObj_sp->setValue(KEY_UINPUT_INPUT_ENABLE, m_bIrInputEnabled);		
				SysSrvSettingsObj_sp->sync();
			}
		}
	}while(FALSE);
}

INT_t QIrInputCtrl::CheckUinputRunning(BOOL_t & bIsRunning)
{
	INT_t iOutRet = ERROR_SUCCESS;
	POSITION pPos;
	P_PROCESS_ITEM_INFO pProcessItemInfo;
	
	bIsRunning = FALSE;

	do
	{
		CONST PROCESS_ITEM_INFO_LIST & ProcessInfoList = m_ProcessList_sp->getProcessList();
		pPos = ProcessInfoList.GetHeadPosition();
		while(pPos)
		{
			pProcessItemInfo = ProcessInfoList.GetNext(OUT pPos);
			do
			{
				if(NULL == pProcessItemInfo)
				{
					break;				
				}
				QString strCmdLine = (LPCSTR)(pProcessItemInfo->strCmdLine);
				QStringList CmdFieldList = strCmdLine.split(' ');
				if(1 > CmdFieldList.size())
				{
					break;
				}
				QStringList DirFieldList = CmdFieldList[0].split('/');
				if(1 > DirFieldList.size())
				{
					break;
				}
				if(0 != DirFieldList[DirFieldList.size()-1].compare("uinput", Qt::CaseInsensitive))
				{
					break;
				}
				bIsRunning = TRUE;
				break;
			}while(FALSE);
			if(bIsRunning)
			{
				break;
			}
		}
	}while(FALSE);

	return iOutRet;
}

void QIrInputCtrl::getInputEnabled(int & iOutRet, int & bEnabled)
{
	iOutRet = ERROR_SUCCESS;

	bEnabled = m_bIrInputEnabled;
}

void QIrInputCtrl::SendIrOut(int iIRCode, int & iOutRet)
{
	iOutRet = ERR_FILE_OPEN_FAIL;
		
	if(NULL==m_pIrBlasterInstance)
		return;
	if(m_pIrBlasterInstance->sendIrCodeOut(iIRCode)==0){
		iOutRet = ERROR_SUCCESS;
	}
}

void QIrInputCtrl::SendIrOut(unsigned int iIRCode, int & iOutRet)
{
	iOutRet = ERR_FILE_OPEN_FAIL;
		
	if(NULL==m_pIrBlasterInstance)
		return;
	if(m_pIrBlasterInstance->sendIrCodeOut(iIRCode)==0){
		iOutRet = ERROR_SUCCESS;
	}
}

void QIrInputCtrl::SendIrLevelOut(QString& iIRLevelCode, int & iOutRet)
{
	
	iOutRet = ERR_FILE_OPEN_FAIL;
	if(Sw_LogDbusCmd)
	{
		LOG("GPIO IR Blasler: iIRLevelCode=%s\n", qPrintable(iIRLevelCode));
	}	
	if(NULL==m_pIrBlasterInstance)
		return;
	if(m_pIrBlasterInstance->sendIrLevelOut(iIRLevelCode.toLatin1().data())==0){
		iOutRet = ERROR_SUCCESS;
	}
		
}

