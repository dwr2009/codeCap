#include "BackendTransactionThread.h"
#include "DbgLogSwitchDef.h"
#include <ErrPrintHelper.h>
#include <SystemServiceAppIf.h>
#include <QtWidgets/QApplication>
#include <unistd.h>

#define	DO_SysSync_DefaultInterval_MS			(20*1000)

QBackendTransactionThread::QBackendTransactionThread()
{
	m_BackendTransThreadId = 0;
	//LOG_BLINE("this.Thrd=%p\n", this->thread());
	this->moveToThread(this);
	//LOG_BLINE("this.Thrd=%p\n", this->thread());
	//LOG_BLINE("SyncTimer.Thrd=%p\n", m_SyncTimer.thread());
	m_SyncTimer.moveToThread(this);
	//LOG_BLINE("SyncTimer.Thrd=%p\n", m_SyncTimer.thread());
}

QBackendTransactionThread::~QBackendTransactionThread()
{

}

void QBackendTransactionThread::OnTimer_Sync()
{
	int iRet;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;

	Q_UNUSED(iRet);

	do
	{
		if(m_BackendTransThreadId != QThread::currentThreadId ())
		{
			LOG_BLINE("BUG,CurThrdId=0x%08x,BackendTransThrdId=0x%08x\n", (UINT_t)QThread::currentThreadId (), (UINT_t)m_BackendTransThreadId);
		}
		
		if(Sw_LogSysIoEvt)
		{
			LOG_BLINE("Sync\n");
		}

		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);	
		if(NULL == pSystemServiceAppIf)
		{
			PRINT_BFILE_LINENO_BUG_STR;
			break;
		}

		CMp7xxGlobalStatus & Mp7xxGlobalStatus = pSystemServiceAppIf->getMp7xxGlobalStatusObj();
		BOOL_t bWritingFs;
		time_t tmLastTimeToWriteFs;
		iRet = Mp7xxGlobalStatus.get_WritingFs_Info(&bWritingFs, &tmLastTimeToWriteFs);
		if(ERROR_SUCCESS != iRet)
		{
			bWritingFs = FALSE;
			tmLastTimeToWriteFs = 0;
			PRINT_BFILE_LINENO_IRET_STR;
		}
		if(bWritingFs)
		{
			time_t tmCurrent = time(NULL);
			if(tmCurrent >= tmLastTimeToWriteFs)
			{
				double tmDiffSec = difftime(tmCurrent, tmLastTimeToWriteFs);			
				if(DO_SysSync_DefaultInterval_MS > (1000 * tmDiffSec))
				{
					m_SyncTimer.start(DO_SysSync_DefaultInterval_MS);
					break;
				}
			}
			else	//tmCurrent < tmLastTimeToWriteFs
			{
				iRet = Mp7xxGlobalStatus.set_WritingFs_Info(FALSE, tmLastTimeToWriteFs);
				if(ERROR_SUCCESS != iRet)
				{
					PRINT_BFILE_LINENO_IRET_STR;
				}
				m_SyncTimer.start(DO_SysSync_DefaultInterval_MS);
				break;
			}
			iRet = Mp7xxGlobalStatus.set_WritingFs_Info(FALSE, tmLastTimeToWriteFs);
			if(ERROR_SUCCESS != iRet)
			{
				PRINT_BFILE_LINENO_IRET_STR;
			}
		}
		
		//LOG_BLINE("before sync\n");
		
		sync();		//will take long time if much pending data needs to be writtten to disk
		m_SyncTimer.start(DO_SysSync_DefaultInterval_MS);
		
		//LOG_BLINE("after sync\n");
	}while(FALSE);
}

void QBackendTransactionThread::run()
{
	int iThreadExitCode = 0;
	bool bRet = false;

	do
	{
		m_BackendTransThreadId = QThread::currentThreadId ();

		{
			//QThread::Priority CurThrdPriority;
			//CurThrdPriority = priority();
			//LOG_BLINE("CurThrdPriority=%d\n", CurThrdPriority);
			setPriority(QThread::LowPriority);
			//CurThrdPriority = priority();
			//LOG_BLINE("CurThrdPriority=%d\n", CurThrdPriority);
		}

		bRet = connect(&m_SyncTimer, SIGNAL(timeout()), this, SLOT(OnTimer_Sync()));
		if(false == bRet)
		{
			LOG_BLINE("connecting OnTimer_Sync err\n");
		}
		//frequent call to sync may make the upgrading untar failing
		//m_SyncTimer.start(DO_SysSync_DefaultInterval_MS);

		iThreadExitCode = exec();
	}while(FALSE);
}

