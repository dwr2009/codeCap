#ifndef	_GPIO_BTN_INPUT_API_H_
#define	_GPIO_BTN_INPUT_API_H_

#define	DBUS_GpioBtnInput_ServiceName			"org.thtfit.GpioBtnInput"
#define DBUS_GpioBtnInput_OBJECT_PATH  			"/org/thtfit/GpioBtnInput"
#define	DBUS_GpioBtnInput_OBJECT_INTERFACE		"org.thtfit.GpioBtnInput"

#define	DBUS_METHOD_Introspect					"Introspect"
#define	DBUS_METHOD_getGpioInterruptSource		"getGpioInterruptSource"
#define	DBUS_METHOD_getGpioDirection			"getGpioDirection"
#define	DBUS_METHOD_setGpioDirection			"setGpioDirection"
#define	DBUS_METHOD_setGpioOutputData			"setGpioOutputData"
#define	DBUS_METHOD_getGpioInputStatus			"getGpioInputStatus"
#define	DBUS_METHOD_setGpioOutputStatus			"setGpioOutputStatus"
#define	DBUS_METHOD_selectInputGpioId			"selectInputGpioId"
#define	DBUS_METHOD_getSelectedInputGpioId		"getSelectedInputGpioId"
#define	DBUS_METHOD_readGpioInputStatus			"readGpioInputStatus"
#define	DBUS_METHOD_selectOutputGpioId			"selectOutputGpioId"
#define	DBUS_METHOD_getSelectedOutputGpioId		"getSelectedOutputGpioId"
#define	DBUS_METHOD_setGpioOutputSingleBit		"setGpioOutputSingleBit"
#define	DBUS_METHOD_setGpioOutputPulseWidth		"setGpioOutputPulseWidth"
#define	DBUS_METHOD_getGpioOutputPulseWidth		"getGpioOutputPulseWidth"
#define	DBUS_METHOD_GenerateGpioOutputTiming	"GenerateGpioOutputTiming"

#define	DBUS_SIG_GpioInputInterrupt				"GpioInputInterrupt"
#define	DBUS_SIG_ServiceStarted					"ServiceStarted"

//GPIO logical direction, 0 -- Input, 1 -- Output
#define	GPIO_LOGICAL_DIR_ALL_INPUT			0x00000000

#define	SysProp_KEY_GpioLogicalDir			"GpioBtnInput.LogicalIoDir"

#endif	//_GPIO_BTN_INPUT_API_H_

