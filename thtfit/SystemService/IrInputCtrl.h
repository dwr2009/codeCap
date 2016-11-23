#ifndef	_IR_INPUT_CTRL_H_
#define	_IR_INPUT_CTRL_H_

#include <QDBusAbstractAdaptor>
#include <QObject>
#include <QtCore/qobjectdefs.h>
#include <BaseTypeDef.h>
#include <QSettings>
#include <QDBusVariant>
#include <QTimer>
#include "SystemSrvApi.h"
#include <QSharedPointer>
#include <ProcessList.h>
#include "Api/IrBlasterInterface.h"
class QIrInputCtrl;

class QDBusIf_IrInputCtrl : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.IrInputCtrl")	
public:
	QDBusIf_IrInputCtrl(QIrInputCtrl * pParentObj);
public slots:
	void EnableDisableInput(int bEnable, int & iOutRet);
	void getInputEnabled(int & iOutRet, int & bEnabled);
	void SendIrOut(int iIRCode, int & iOutRet);
	void SendIrOut(unsigned int iIRCode, int & iOutRet);
    void SendIrLevelOut(QString iIRLevelCode, int & iOutRet);
signals:
private:
	QIrInputCtrl * m_pIrInputCtrl;
};

class QIrInputCtrl : public QObject
{
	Q_OBJECT		
	friend class QDBusIf_IrInputCtrl;
public:
	QIrInputCtrl();
	~QIrInputCtrl();
	INT_t InitInstance();
	INT_t ExitInstance();
	void EnableDisableInput(int bEnable, int & iOutRet, bool bSaveSettings = TRUE);
	void getInputEnabled(int & iOutRet, int & bEnabled);
	void SendIrOut(int iIRCode, int & iOutRet);
	void SendIrOut(unsigned int iIRCode, int & iOutRet);
	void SendIrLevelOut(QString& iIRLevelCode, int & iOutRet);
protected:
	IrBlasterInterface *m_pIrBlasterInstance;
private:
	INT_t CheckUinputRunning(BOOL_t & bIsRunning);	
signals:
private:
	QSharedPointer <QDBusIf_IrInputCtrl> m_DbusIf_IrInputCtrl_sp;
	INT_t m_bIrInputEnabled;
	QSharedPointer <CProcessList> m_ProcessList_sp;
};

#endif	//_IR_INPUT_CTRL_H_

