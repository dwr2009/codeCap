#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <BaseErrDef.h>
#include <ErrPrintHelper.h>

#include <QtWidgets/QApplication>
#include <QDBusError>

#include "PowerMate.h"
#include "SystemServiceAppIf.h"
#include "SysBaseSrvApi.h"

#include <StackBufString.h>

#ifdef LOGERR
#undef LOGERR
#endif

#ifdef LOGWAR
#undef LOGWAR
#endif

#ifdef LOGINF
#undef LOGINF
#endif

#define LOGERR(fmt,...) printf("ERROR: %s(%d) " fmt,__func__,__LINE__,##__VA_ARGS__)
#if 0//For debug
#define LOGWAR(fmt,...) printf("WARNING: %s(%d) " fmt,__func__,__LINE__,##__VA_ARGS__)
#define LOGINF(fmt,...) printf("%s(%d) " fmt,__func__,__LINE__,##__VA_ARGS__)
#else
#define LOGWAR(fmt,...)
#define LOGINF(fmt,...)
#endif

static const char *power_mate_valid_prefix[POWER_MATE_NUM_VALID_PREFIXES] = {
  "Griffin PowerMate",
  "Griffin SoundKnob"
};

static const unsigned int PowerMateKeyMap[]={
	KEY_MUTE,
	KEY_F4,      //VOLUMEDOWN,
	KEY_F5       //VOLUMEUP
};

QDBusIf_PowerMate::QDBusIf_PowerMate(QPowerMate *pParentObj) : QDBusAbstractAdaptor(pParentObj)
{
	mParent = pParentObj;

	setAutoRelaySignals(true);
}

int QDBusIf_PowerMate::GetEnableGlobalControl(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->GetEnableGlobalControl();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::SetEnabledGlobalControl(int b)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->SetEnabledGlobalControl(b);
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::get_brightness(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->get_brightness();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::set_brightness(int value)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->set_brightness(value);
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::get_pulse_speed(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->get_pulse_speed();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::set_pulse_speed(int value)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->set_pulse_speed(value);
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::get_pulse_mode(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->get_pulse_mode();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::set_pulse_mode(int value)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->set_pulse_mode(value);
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::get_asleep(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->get_asleep();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::set_asleep(int value)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->set_asleep(value);
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::get_awake(void)
{
	int iOutRet = -1;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			break;
		}

		iOutRet = mParent->get_awake();
	}while(FALSE);

	return iOutRet;
}

int QDBusIf_PowerMate::set_awake(int value)
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( NULL == mParent ){
			LOGERR("Invalid mParent NULL\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		iOutRet = mParent->set_awake(value);
	}while(FALSE);

	return iOutRet;
}

QPowerMate::QPowerMate()
{
	mEnableGlobalControl = TRUE;

	mPowerMateFd = -1;
	mUinputFd = -1;

	mPowerMatePulseLEDStatus.Level  = 0x80;
	mPowerMatePulseLEDStatus.Speed  = 255;
	mPowerMatePulseLEDStatus.Style  = 0;
	mPowerMatePulseLEDStatus.Asleep = 1;
	mPowerMatePulseLEDStatus.Awake  = 0;
}

QPowerMate::~QPowerMate()
{
	ExitInstance();
}

void QPowerMate::OnPowerMatePlugIn(void)
{
	LOGINF("PlugIn\n");
	Open();
}

void QPowerMate::OnPowerMatePlugOut(void)
{
	LOGINF("PlugOut\n");
	Close();
}

int QPowerMate::GetEnableGlobalControl(void)
{
	int iRet, iEnabled=1, iOutRet;

	do{
		DECLARE_CLS_STACK_BUF_STRING(strPowerMateEnableGlobalControl, 32);
		iRet = SysProp_get(INI_KEY_POWERMATE_ENABLE_GLOBAL_CONTROL, OUT strPowerMateEnableGlobalControl);
		if(ERROR_SUCCESS == iRet){
			iEnabled = QString(strPowerMateEnableGlobalControl).toInt();
		}else{
			LOGWAR("SysProp_get(%s) failed, iRet=%d\n",INI_KEY_POWERMATE_ENABLE_GLOBAL_CONTROL,iRet);
			iEnabled = 1;
		}
	}while(FALSE);
		
	iOutRet = iEnabled;
	mEnableGlobalControl = iEnabled ? TRUE : FALSE;
	
	return iOutRet;
}

int QPowerMate::SetEnabledGlobalControl(int b)
{
	int iRet, iOutRet=ERROR_SUCCESS;

	do{
		iRet = SysProp_setPersistent(INI_KEY_POWERMATE_ENABLE_GLOBAL_CONTROL, b);
		if(ERROR_SUCCESS != iRet){
			LOGERR("SysProp_setPersistent(%s) failed, iRet=%d\n",INI_KEY_POWERMATE_ENABLE_GLOBAL_CONTROL,iRet);
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;	
		}

		mEnableGlobalControl = b ? TRUE : FALSE;
	}while(FALSE);
		
	return iOutRet;
}

int QPowerMate::InitInstance()
{
	int iOutRet = ERROR_SUCCESS;
	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;
	bool bRet;

	do{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL == pSystemServiceAppIf){
			LOGERR("pSystemServiceAppIf is NULL\n");
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
		if(DBusConn_sp.isNull()){
			LOGERR("DBusConn_sp is NULL\n");
			iOutRet = ERROR_INVALID_STATE;
			break;
		}

		mDBusIfPowerMate_sp = QSharedPointer <QDBusIf_PowerMate> (new QDBusIf_PowerMate(this));
		if(mDBusIfPowerMate_sp.isNull()){
			LOGERR("Out of memory\n");
			iOutRet = ERROR_OUT_OF_MEMORY;
			break;
		}

		bRet = DBusConn_sp->registerObject(DBUS_PATH_SysSrv_PowerMate,this);
		if( false == bRet ){
			iOutRet = ERR_REGISTER_FAIL;
			QDBusError QDBusErr = DBusConn_sp->lastError();
			if(QDBusErr.isValid()){
				LOGERR("ConnDBus,ErrType:%d,%s,%s\n", QDBusErr.type(), qPrintable(QDBusErr.name()), qPrintable(QDBusErr.message()));
			}else{
				LOGERR("Failed to reg dbus obj %s\n", DBUS_PATH_SysSrv_PowerMate);
			}
			break;
		}

		LOGINF("DBUS registerObject(DBUS_PATH_SysSrv_PowerMate)\n");
		GetEnableGlobalControl();	

		bRet = DBusConn_sp->connect("", DBUS_PATH_SysSrv_PowerMate, DbusIf_SysSrv_PowerMate, "OnPowerMatePlugIn", this, SLOT(OnPowerMatePlugIn()));
		if(false == bRet){
			LOGERR("Connect sinal(OnPowerMatePlugIn) failed\n");
		}

		bRet = DBusConn_sp->connect("", DBUS_PATH_SysSrv_PowerMate, DbusIf_SysSrv_PowerMate, "OnPowerMatePlugOut", this, SLOT(OnPowerMatePlugOut()));
		if(false == bRet){
			LOGERR("Connect sinal(OnPowerMatePlugOut) failed\n");
		}

		iOutRet = Open();

	}while(FALSE);

	return iOutRet;
}

int QPowerMate::ExitInstance()
{
	int iOutRet = ERROR_SUCCESS;

	CSystemServiceAppIf * pSystemServiceAppIf = NULL;
	QSharedPointer <QDBusConnection> DBusConn_sp;

	do
	{
		pSystemServiceAppIf = dynamic_cast <CSystemServiceAppIf *>(qApp);
		if(NULL != pSystemServiceAppIf){
			DBusConn_sp = pSystemServiceAppIf->getDBusConnection();
			if(FALSE == DBusConn_sp.isNull()){
				if(DBusConn_sp->isConnected()){
					LOGINF("DBUS unregisterObject(DBUS_PATH_SysSrv_PowerMate)\n");
					DBusConn_sp->unregisterObject(DBUS_PATH_SysSrv_PowerMate); 		
				}
			}
		}

		iOutRet = Close();
	}while(FALSE);
	
	return iOutRet;
}

int QPowerMate::Open()
{
	int iOutRet = ERROR_SUCCESS;

	do{
		if( 0 <= mUinputFd ){
			CloseUinput();//Close first
		}

		mUinputFd = OpenUinput();
		if( 0 > mUinputFd ){
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		iOutRet = SetupUinputDevice();
		if( ERR_SUCCESS != iOutRet ){
			break;
		}

		if( 0 <= mPowerMateFd ){
			ClosePowerMate();//Close first
		}

		mPowerMateFd = OpenPowerMate();
		if( 0 > mPowerMateFd ){
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}

		iOutRet = ListenPowerMateEvent();
		if( ERR_SUCCESS == iOutRet ){
			LOGINF("Open PowerMate devices ok\n");
		}

	}while(FALSE);

	return iOutRet;
}

int QPowerMate::Close()
{
	ClosePowerMate();
	CloseUinput();

	return ERROR_SUCCESS;
}

int QPowerMate::ListenPowerMateEvent()
{
	int iOutRet = ERROR_SUCCESS;
	bool bRet;

	do{
		if(0 > mPowerMateFd){
			LOGERR("Invalid PowerMate fd(%d)\n", mPowerMateFd);
			iOutRet = ERR_INV_PARAM;
			break;
		}

		mPowerMateEventFdNotifier_sp = QSharedPointer <QSocketNotifier> (new QSocketNotifier(mPowerMateFd, QSocketNotifier::Read, this));
		if( mPowerMateEventFdNotifier_sp.isNull() ){
			LOGERR("Out of memory\n");
			iOutRet = ERR_OUT_OF_MEMORY;
			break;
		}

		bRet = connect(&(*mPowerMateEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(OnPowerMateEvent(int)));
		if( false == bRet ){
			LOGERR("Connect PowerMateEventFdNotifier failed\n");
			iOutRet = ERR_INV_PARAM;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int QPowerMate::OpenPowerMate()
{
	char devName[256],devInfo[255];
	int i,j,fd=-1,isFound=0;

	do{
		//Scan PowerMate /dev
		for(i=0; i<POWER_MATE_NUM_EVENT_DEVICES; i++){
			snprintf(devName, sizeof(devName)-1, POWER_MATE_DEV_EVENT, i);
			fd = open(devName, O_RDWR);

			if(fd < 0){
				LOGWAR("Unable to open \"%s\": %s\n", devName, strerror(errno));
				fd = -1;
				break;
			}

			if( ioctl(fd, EVIOCGNAME(sizeof(devInfo)), devInfo) < 0 ){
				LOGERR("\"%s\": EVIOCGNAME failed: %s\n", devName, strerror(errno));
				close(fd);
				fd = -1;
				break;
			}

			//It's the correct device if the prefix matches what we expect it to be:
			for(j=0; j<POWER_MATE_NUM_VALID_PREFIXES; j++){
				if( !strncasecmp(devInfo, power_mate_valid_prefix[j], strlen(power_mate_valid_prefix[j])) ){
					isFound = 1;
					break;
				}
			}

			if( 1 == isFound ){
				LOGINF("Found PowerMate devices is %s\n",devName);
				emit on_powermate_init();
				break;//scan finished
			}else{
				close(fd);
				fd = -1;
			}
		}
	}while(FALSE);

	return fd;
}

int QPowerMate::ClosePowerMate()
{
	bool bRet;

	if(FALSE == mPowerMateEventFdNotifier_sp.isNull()){
		mPowerMateEventFdNotifier_sp->setEnabled(FALSE);
		bRet = QObject::disconnect(&(*mPowerMateEventFdNotifier_sp), SIGNAL(activated(int)), this, SLOT(OnPowerMateEvent(int)));
		if(FALSE == bRet){
			LOGERR("PowerMateEventFdNotifier.disconnect failed\n");
		}
		mPowerMateEventFdNotifier_sp.clear();
	}

	if( 0 <= mPowerMateFd ){
		close(mPowerMateFd);
		mPowerMateFd = -1;
		
		emit on_powermate_uninit();
		LOGINF("Close PowerMate\n");
	}

	return ERROR_SUCCESS;
}

int QPowerMate::OpenUinput()
{
	char devName[256];
	int fd=-1;
	int iOutRet = ERROR_SUCCESS;

	do{
		snprintf( devName, sizeof(devName)-1, UINPUT_DEV_NAME );
		fd = open( devName, O_WRONLY | O_NDELAY );

		if( 0 > fd ){
			LOGERR("Unable to open \"%s\": %s\n", devName, strerror(errno));
			iOutRet = ERROR_FILE_OPEN_FAIL;
			break;
		}
	}while(FALSE);

	return fd;
}

int QPowerMate::SetupUinputDevice(void)
{
	int i = 0, iRet, count;
	int iOutRet = ERROR_SUCCESS;

	do{
		if(0 > mUinputFd){
			LOGERR("Invalid uinput fd(%d)\n", mUinputFd);
			iOutRet = ERR_INV_PARAM;
			break;
		}

		memset(&mUinputUserDev, 0, sizeof(mUinputUserDev));
		strncpy(mUinputUserDev.name, "PowerMate Input", UINPUT_MAX_NAME_SIZE);
		mUinputUserDev.id.version = 1; /* anything */
		mUinputUserDev.id.bustype = BUS_VIRTUAL;

		//support KEY event
		iRet = ioctl(mUinputFd, UI_SET_EVBIT, EV_KEY);
		if(0 > iRet){
			LOGERR("UI_SET_EVBIT ioctl errno: %d %s\n",errno,strerror(errno));
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}

		//Set customized keys
		count = (int)(sizeof(PowerMateKeyMap) / sizeof(PowerMateKeyMap[0]));
		//LOGINF("count=%d\n",count);
		for (i = 0; i < count; i++) {
			iRet = ioctl(mUinputFd, UI_SET_KEYBIT, PowerMateKeyMap[i]);
			if(0 > iRet){
				LOGERR("UI_SET_KEYBIT ioctl errno: %d %s\n",errno,strerror(errno));
				iOutRet = ERROR_IOCTL_FAILED;
				break;
			}
		}

		iRet = write(mUinputFd, &mUinputUserDev, sizeof(mUinputUserDev));
		if(0 > iRet){
			LOGERR("SetUserDev ioctl errno: %d %s\n",errno,strerror(errno));
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}

		iRet = ioctl(mUinputFd, UI_DEV_CREATE);
		if(0 > iRet){
			LOGERR("Create uinput device ioctl errno: %d %s\n",errno,strerror(errno));
			iOutRet = ERROR_IOCTL_FAILED;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int QPowerMate::CloseUinput()
{
	int iRet, iOutRet = ERROR_SUCCESS;

	if( 0 <= mUinputFd ){
		iRet = ioctl(mUinputFd, UI_DEV_DESTROY);
		if(0 != iRet){
			LOGERR("UI_DEV_DESTROY ioctl errno: %d %s\n",errno,strerror(errno));
		}

		close( mUinputFd );
		mUinputFd = -1;

		LOGINF("Close uinput\n");
	}

	return iOutRet;
}

int QPowerMate::SendKeyToUinput(int key)
{
	int iRet,iOutRet = ERROR_SUCCESS;

	do{
		if( FALSE == mEnableGlobalControl ){
			LOGINF("mEnableGlobalControl=%d\n",mEnableGlobalControl);
			break;
		}

		if( 0 > mUinputFd ){
			LOGERR("Uinput fd error");
			iOutRet = ERR_INV_PARAM;
			break;
		}

		/* Key pressed */
		memset(&mUinputEvent, 0, sizeof(mUinputEvent));
		gettimeofday(&mUinputEvent.time, NULL);

		mUinputEvent.type = EV_KEY;
		mUinputEvent.code = key;
		mUinputEvent.value = 1;
		iRet = write(mUinputFd, &mUinputEvent, sizeof(mUinputEvent));
		if( sizeof(mUinputEvent) != iRet ){
			LOGWAR("Write uinput key fails, erros=%s\n",strerror(errno));
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}

		mUinputEvent.type = EV_SYN;
		mUinputEvent.code = SYN_REPORT;
		mUinputEvent.value = 0;
		iRet = write(mUinputFd, &mUinputEvent, sizeof(mUinputEvent));
		if( sizeof(mUinputEvent) != iRet ){
			LOGWAR("Write uinput key fails, erros=%s\n",strerror(errno));
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}

		/* Key released */
		memset(&mUinputEvent, 0, sizeof(mUinputEvent));
		gettimeofday(&mUinputEvent.time, NULL);

		mUinputEvent.type = EV_KEY;
		mUinputEvent.code = key;
		mUinputEvent.value = 0;
		iRet = write(mUinputFd, &mUinputEvent, sizeof(mUinputEvent));
		if( sizeof(mUinputEvent) != iRet ){
			LOGWAR("Write uinput key fails, erros=%s\n",strerror(errno));
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}

		mUinputEvent.type = EV_SYN;
		mUinputEvent.code = SYN_REPORT;
		mUinputEvent.value = 0;
		iRet = write(mUinputFd, &mUinputEvent, sizeof(mUinputEvent));
		if( sizeof(mUinputEvent) != iRet ){
			LOGWAR("Write uinput key fails, erros=%s\n",strerror(errno));
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}

		LOGINF("SendKeyToUinput(0x%04X)\n",key);
	}while(FALSE);

	return iOutRet;
}

int QPowerMate::SetPowerMatePulseLEDStatus(ePowerMatePulseLEDFlag setFlag,int value)
{
	int iRet,iOutRet = ERROR_SUCCESS;
	struct input_event ev;
	tPowerMatePulseLEDStatus ledInfo;

	do{
		if(0 > mPowerMateFd){
			//LOGERR("Invalid PowerMate fd(%d)\n", mPowerMateFd);
			iOutRet = ERR_INV_STATE;
			break;
		}

		memcpy(&ledInfo,&mPowerMatePulseLEDStatus,sizeof(ledInfo));
  		switch( setFlag ){
			case ePowerMatePulseLED_Level:
				value &= 0xFF;
				ledInfo.Level = value;
				break;
			case ePowerMatePulseLED_Speed:
				if(value < 0){
					value = 0;
				}
				if(value > 510){
					value = 510;
				}
				ledInfo.Speed = value;
				break;
			case ePowerMatePulseLED_Style:
				if(value < 0){
					value = 0;
				}
				if(value > 2){
					value = 2;
				}
				ledInfo.Style = value;
				break;
			case ePowerMatePulseLED_Asleep:
				value = !!value;
				ledInfo.Asleep = value;
				break;
			case ePowerMatePulseLED_Awake:
				value = !!value;
				ledInfo.Awake = value;
				break;
			default:
				LOGERR("Invalid SetFlag (%d)\n", setFlag);
				iOutRet = ERR_INV_PARAM;
				break;
  		}

		if( ERR_SUCCESS != iOutRet ){
			break;
		}

		memset(&ev, 0, sizeof(ev));

		ev.type = EV_MSC;
		ev.code = MSC_PULSELED;
		ev.value = ( ledInfo.Level        ) |
		           ( ledInfo.Speed  << 8  ) |
		           ( ledInfo.Style  << 17 ) |
		           ( ledInfo.Asleep << 19 ) |
		           ( ledInfo.Awake  << 20 );

		iRet = write(mPowerMateFd, &ev, sizeof(ev));
		if( sizeof(ev) != iRet ){
			LOGWAR("Write PowerMate LED fails, erros= %d, %s\n", errno, strerror(errno));
			iOutRet = ERROR_FILE_WRITE_FAIL;
			break;
		}
	}while(FALSE);

	return iOutRet;
}

int QPowerMate::GetPowerMatePulseLEDStatus(ePowerMatePulseLEDFlag getFlag)
{
	int value=-1;

	do{
  		switch( getFlag ){
			case ePowerMatePulseLED_Level:
				value = mPowerMatePulseLEDStatus.Level;
				break;
			case ePowerMatePulseLED_Speed:
				value = mPowerMatePulseLEDStatus.Speed;
				break;
			case ePowerMatePulseLED_Style:
				value = mPowerMatePulseLEDStatus.Style;
				break;
			case ePowerMatePulseLED_Asleep:
				value = mPowerMatePulseLEDStatus.Asleep;
				break;
			case ePowerMatePulseLED_Awake:
				value = mPowerMatePulseLEDStatus.Awake;
				break;
			default:
				LOGERR("Invalid getFlag (%d)\n", getFlag);
				break;
  		}
	}while(FALSE);

	return value;
}

void QPowerMate::OnPowerMateEvent(int iFd)
{
	int i, readLen, eventCount;
	bool isFatalError=false;

	do{
		if( 0 > iFd ){
			LOGERR("Fd error");
			break;
		}

		readLen = read(iFd, mPowerMateEventBuffer, sizeof(mPowerMateEventBuffer));
		if( 0 >= readLen ){
			switch( errno ){
				case 0:       /*Success*/
				case EINTR:   /*A signal occurred before any requested event,Interrupted system call*/
				case EAGAIN:  /*Resource temporarily unavailable*/
					LOGERR("PowerMate event read(%d) failed, errno %d: %s\n", readLen, errno, strerror(errno));
					break;
				case EPIPE:   /*Broken pipe*/
				case ENODEV:  /*No such device*/
				case EBADFD:  /*File descriptor in bad state*/
				case EFAULT:  /*The array given as argument was not contained in the calling program's address space*/
				case EINVAL:  /*The nfds value exceeds the RLIMIT_NOFILE value*/
				case ENOMEM:  /*There was no space to allocate file descriptor tables*/
				case ENOENT:  /*Input/output error*/
				case EIO:     /*No such file or directory*/
				default:
					LOGWAR("PowerMate event read(%d) Fatal errno(%d: %s)\n", readLen, errno, strerror(errno));
					isFatalError=true;
					break;
			}
			break;
		}

		eventCount = readLen / sizeof(struct input_event);
		for(i=0; i<eventCount; i++){
			OnPowerMateEventDispatch(&mPowerMateEventBuffer[i]);
		}
	}while(FALSE);

	if( isFatalError ){
		Close();
	}
}

void QPowerMate::OnPowerMateEventDispatch(struct input_event *ev)
{
	do{
		switch(ev->type){
			case EV_MSC:
				LOGINF("The LED pulse settings were changed; code=0x%04x, value=0x%08x\n", ev->code, ev->value);
				OnPowerMatePulseLEDStatusChange((unsigned int)ev->value);//event to user
				break;
			case EV_REL:
				if(ev->code != REL_DIAL){
					LOGWAR("Warning: unexpected rotation event; ev->code = 0x%04x\n", ev->code);
				}else{
					OnPowerMateRotate((int)ev->value);
				}
				break;
			case EV_KEY:
				if(ev->code != BTN_0){
      				LOGWAR("Warning: unexpected key event; ev->code = 0x%04x\n", ev->code);
      				break;
      			}
				//LOGINF("Button was %s\n", ev->value? "pressed":"released");
				if( ev->value ){
					OnPowerMateKeyDown(ev->code);
				}else{
					OnPowerMateKeyUp(ev->code);
				}
				break;
			default:
				//LOGWAR("Warning: unexpected event type; ev->type = 0x%04x\n", ev->type);
				break;
		}
	}while(FALSE);
}

void QPowerMate::OnPowerMateKeyDown(int keyVal)
{
	switch( keyVal ){
		case BTN_0:
			emit on_button_press(keyVal);
			SendKeyToUinput(KEY_MUTE);
			break;
		default:
			LOGWAR("Warning: unkown key value = 0x%04x\n", keyVal);
	}
}

void QPowerMate::OnPowerMateKeyUp(int keyVal)
{
	switch( keyVal ){
		case BTN_0:
			emit on_button_release(keyVal);
			break;
		default:
			LOGWAR("Warning: unkown key value = 0x%04x\n", keyVal);
	}
}

void QPowerMate::OnPowerMateRotate(int val)
{
	emit on_rotate(val);
	
	if( 0 < val ){
		SendKeyToUinput(KEY_F4);
	}else{
		SendKeyToUinput(KEY_F5);
	}

	//LOGINF("Rotated %d, Offset %d\n", (int)val, mPowerMateAbsOffset);
}

void QPowerMate::OnPowerMatePulseLEDStatusChange(int value)
{
#if 0
	tPowerMatePulseLEDStatus ledStatus;//avoid user to change globals mPowerMatePulseLEDStatus

	memcpy(&ledStatus,&mPowerMatePulseLEDStatus,sizeof(ledStatus));

	LOGINF("emit LED Status Change\n");
#endif

	mPowerMatePulseLEDStatus.Level  = (value & 0xFF);
	mPowerMatePulseLEDStatus.Speed  = (value >> 8 ) & 0x1FF;
	mPowerMatePulseLEDStatus.Style  = (value >> 17) & 0x03;
	mPowerMatePulseLEDStatus.Asleep = (value >> 19) & 0x01;
	mPowerMatePulseLEDStatus.Awake  = (value >> 20) & 0x01;

	LOGINF("LED Status:\n" \
	       "	Level:  %d\n" \
	       "	Speed:  %d\n" \
	       "	Style:  %d\n" \
	       "	Asleep: %d\n" \
	       "	Awake:  %d\n",
	       mPowerMatePulseLEDStatus.Level,
	       mPowerMatePulseLEDStatus.Speed,
		   mPowerMatePulseLEDStatus.Style,
		   mPowerMatePulseLEDStatus.Asleep,
	       mPowerMatePulseLEDStatus.Awake);
/*	
		ev.value = ( ledInfo.Level        ) |
		           ( ledInfo.Speed  << 8  ) |
		           ( ledInfo.Style  << 17 ) |
		           ( ledInfo.Asleep << 19 ) |
		           ( ledInfo.Awake  << 20 );*/

	emit on_led_change(value);
}

int QPowerMate::get_brightness(void)
{
	return GetPowerMatePulseLEDStatus(ePowerMatePulseLED_Level);
}

int QPowerMate::set_brightness(int value)
{
	return SetPowerMatePulseLEDStatus(ePowerMatePulseLED_Level,value);
}

int QPowerMate::get_pulse_speed(void)
{
	return GetPowerMatePulseLEDStatus(ePowerMatePulseLED_Speed);
}

int QPowerMate::set_pulse_speed(int value)
{
	return SetPowerMatePulseLEDStatus(ePowerMatePulseLED_Speed,value);
}

int QPowerMate::get_pulse_mode(void)
{
	return GetPowerMatePulseLEDStatus(ePowerMatePulseLED_Style);
}

int QPowerMate::set_pulse_mode(int value)
{
	return SetPowerMatePulseLEDStatus(ePowerMatePulseLED_Style,value);
}

int QPowerMate::get_asleep(void)
{
	return GetPowerMatePulseLEDStatus(ePowerMatePulseLED_Asleep);
}

int QPowerMate::set_asleep(int value)
{
	return SetPowerMatePulseLEDStatus(ePowerMatePulseLED_Asleep,value);
}

int QPowerMate::get_awake(void)
{
	return GetPowerMatePulseLEDStatus(ePowerMatePulseLED_Awake);
}

int QPowerMate::set_awake(int value)
{
	return SetPowerMatePulseLEDStatus(ePowerMatePulseLED_Awake,value);
}
