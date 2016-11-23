#ifndef _POWER_MATE_H
#define _POWER_MATE_H

#include <QSharedPointer>
#include <QDBusConnection>
#include <BaseTypeDef.h>
#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QSocketNotifier>
#include "SystemSrvApi.h"

#include <linux/input.h>
#include <linux/uinput.h>

#define POWER_MATE_DEV_EVENT                    "/dev/event%d"
#define POWER_MATE_EVENT_BUFFER_COUNT           32
#define POWER_MATE_NUM_VALID_PREFIXES           2
#define POWER_MATE_NUM_EVENT_DEVICES            16

#define UINPUT_DEV_NAME                         "/dev/uinput"

#define INI_KEY_POWERMATE_ENABLE_GLOBAL_CONTROL "PowerMateEnableGlobalControl"

typedef enum{
	ePowerMatePulseLED_Level = 0, //LED intensity(brightness) with pulsing disabled (0-255)
	ePowerMatePulseLED_Speed,     //Pulsing speed (0-510); around 255 works best
	ePowerMatePulseLED_Style,     //Pulsing style (0, 1 or 2) also called mode
	ePowerMatePulseLED_Asleep,    //Enable,disable pulsing whilst host is asleep
	ePowerMatePulseLED_Awake,     //Enable,disable pulsing whilst host is awake
}ePowerMatePulseLEDFlag,*ePowerMatePulseLEDFlag_p;

typedef struct{
	int Level;
	int Speed;
	int Style;
	int Asleep;
	int Awake;
}tPowerMatePulseLEDStatus,*tPowerMatePulseLEDStatus_p;

class QPowerMate;
class QDBusIf_PowerMate : public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "org.thtfit.SystemService.PowerMate")

private:
	QPowerMate *mParent;

public:
	QDBusIf_PowerMate(QPowerMate *pParentObj);

public slots:
	int GetEnableGlobalControl(void);
	int SetEnabledGlobalControl(int b);

	int get_brightness(void);
	int set_brightness(int value);

	int get_pulse_speed(void);
	int set_pulse_speed(int value);

	int get_pulse_mode(void);
	int set_pulse_mode(int value);

	int get_asleep(void);
	int set_asleep(int value);

	int get_awake(void);
	int set_awake(int value);
	
signals:
	void on_powermate_init(void);
	void on_powermate_uninit(void);

	void on_button_press(int KeyVal);
	void on_button_release(int KeyVal);
	
	void on_led_change(unsigned int ledStatus);
	void on_rotate(int offset);	
};

class QPowerMate : public QObject
{
	Q_OBJECT
	friend class QDBusIf_PowerMate;

private:
	int mPowerMateFd;
	
	tPowerMatePulseLEDStatus mPowerMatePulseLEDStatus;

	QSharedPointer <QDBusIf_PowerMate> mDBusIfPowerMate_sp;
	QSharedPointer <QSocketNotifier> mPowerMateEventFdNotifier_sp;
	
	struct input_event mPowerMateEventBuffer[POWER_MATE_EVENT_BUFFER_COUNT];
		
	int ListenPowerMateEvent();
	
	void OnPowerMateEvent(struct input_event *ev);
	void OnPowerMateEventDispatch(struct input_event *ev);
	
	void OnPowerMateKeyDown(int keyVal);
	void OnPowerMateKeyUp(int keyVal);
	
	void OnPowerMateRotate(int val);
	
	void OnPowerMatePulseLEDStatusChange(int value);
	
	int GetPowerMatePulseLEDStatus(ePowerMatePulseLEDFlag getFlag);
	int SetPowerMatePulseLEDStatus(ePowerMatePulseLEDFlag setFlag,int value);

	int mUinputFd;
	BOOL mEnableGlobalControl;
	
	struct input_event mUinputEvent;
	struct uinput_user_dev mUinputUserDev;
	
	int OpenUinput();
	int CloseUinput();
	
	int SetupUinputDevice(void);
	int SendKeyToUinput(int key);

	int OpenPowerMate();
	int ClosePowerMate();

	int Open();
	int Close();

public:
	QPowerMate();
	~QPowerMate();

	int InitInstance();
	int ExitInstance();

	int GetEnableGlobalControl(void);
	int SetEnabledGlobalControl(int b);

	int get_brightness(void);
	int set_brightness(int value);

	int get_pulse_speed(void);
	int set_pulse_speed(int value);

	int get_pulse_mode(void);
	int set_pulse_mode(int value);

	int get_asleep(void);
	int set_asleep(int value);

	int get_awake(void);
	int set_awake(int value);

public slots:
	void OnPowerMateEvent(int iFd);

	void OnPowerMatePlugIn(void);
	void OnPowerMatePlugOut(void);

signals:
	void on_powermate_init(void);
	void on_powermate_uninit(void);
	
	void on_button_press(int KeyVal);
	void on_button_release(int KeyVal);

	void on_led_change(unsigned int ledStatus);
	void on_rotate(int offset);
};
#endif