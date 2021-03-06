/*
 * Note: this file originally auto-generated by mib2c using
 *        : mib2c.scalar.conf 17337 2009-01-01 14:28:29Z magfr $
 */
#ifndef GPIO_H
#define GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
} GpioConfig;

void init_gpio(void);

#ifdef __cplusplus
}
#endif

Netsnmp_Node_Handler handle_gpioMode;
Netsnmp_Node_Handler handle_j12GPIO1;
Netsnmp_Node_Handler handle_j12GPIO2;
Netsnmp_Node_Handler handle_j12All;
Netsnmp_Node_Handler handle_j12PulseWidth;
Netsnmp_Node_Handler handle_j12PulseHigh;
Netsnmp_Node_Handler handle_j12PulseLow;
Netsnmp_Node_Handler handle_j24SelectInput;
Netsnmp_Node_Handler handle_j24ReadInput;
Netsnmp_Node_Handler handle_j24ReadAll;
Netsnmp_Node_Handler handle_j24SelectOutput;
Netsnmp_Node_Handler handle_j24Output;
Netsnmp_Node_Handler handle_j24WriteAll;
Netsnmp_Node_Handler handle_j24InvertInput;
Netsnmp_Node_Handler handle_j24InterruptControl;
Netsnmp_Node_Handler handle_gpioTrapAddress;
Netsnmp_Node_Handler handle_gpioDirection;
Netsnmp_Node_Handler handle_gpioOutputData;
Netsnmp_Node_Handler handle_gpioLedStatus;

#endif                          /* GPIO_H */
