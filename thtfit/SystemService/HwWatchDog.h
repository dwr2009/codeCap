#if	!defined(_HwWatchDog_h_)
#define	_HwWatchDog_h_

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <BaseErrDef.h>
#include <BaseTypeDef.h>
#include <QSharedPointer>
#include <UnixDatagramSocket.h>
#include <QTimer>

class QHwWatchDog;

class QDBusIf_HwWatchDog : public QDBusAbstractAdaptor
{
	Q_OBJECT		
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.HwWatchDog")	
public:
	QDBusIf_HwWatchDog(QHwWatchDog * pParentObj);
	~QDBusIf_HwWatchDog();
public slots:
	void FeedWatchDog(int & iOutRet);
	void WD_SetParam(int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs, int & iOutRet);
signals:
private:
	QHwWatchDog * m_pHwWatchDog;
};

class QHwWatchDog : public QObject
{
	Q_OBJECT		
	friend class QDBusIf_HwWatchDog;
public:
	QHwWatchDog();
	~QHwWatchDog();
	INT_t InitInstance();
	INT_t ExitInstance();
	INT_t FeedWatchDog();
	INT_t WD_SetParam(const int bAutoFeedWatchDog, unsigned int nWatchDogTimeoutMs);
public slots:
	void On_FeedWatchDog();
private:
	QSharedPointer <QDBusIf_HwWatchDog> m_DbusIf_HwWatchDog_sp;
	QSharedPointer <CUnixDatagramSocket> m_WdCtrlSckClient_sp;
	BOOL_t m_bWdCtrlSckConnected;
	BOOL_t m_bNeedFeedWatchDog;
	unsigned int m_TotalWatchDogTimeoutMs;
	UINT64_t m_WatchDogStartTimeMs;
	QTimer m_FeedWdTimer;
	struct sockaddr_un m_WdSckAddr;
};

#endif	//_HwWatchDog_h_

