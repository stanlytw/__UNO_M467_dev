/*
  Copyright (c) 2011 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/
#ifndef _PINS_ARDUINO_H_
#define _PINS_ARDUINO_H_

#define USE_BoardToPin


#if defined(__NUC131__)
#define NOT_AN_INTERRUPT -1
//#define digitalPinToInterrupt(p) ((p) == 2 ? 0 : ((p) == 3 ? 1 : ((p) >= 18 && (p) <= 21 ? 23 - (p) : NOT_AN_INTERRUPT)))
#define digitalPinToInterrupt(p) (((p) >= 2 && (p) <= 13)? (p) : NOT_AN_INTERRUPT)
#ifndef SERIAL_PORT_MONITOR
  #define SERIAL_PORT_MONITOR Serial
#endif
#endif

#if defined(__M252__)
#include "Pins_M252_arduino.h"

#elif defined(__M480__)
#include "Pins_M480_arduino.h"

#elif defined(__M460__) && defined(__M467SJHAN__)
#include "Pins_M467_UNO_arduino.h"

#elif defined(__M460__) && defined(__M467HJHAN__)
#include "Pins_M460_arduino.h"

#elif defined(__M032KG__)
#include "Pins_M032_arduino.h"
#else

/* Types used for the tables below */
typedef struct _PinType
{
#if defined(__M451__) | defined(__NANO100__) | defined(__NANO1X2__) | defined(__M032BT__)
    uint32_t num;
    uint32_t type;
#elif defined(__NUC240__)
    uint32_t num;
    uint32_t type;
    uint32_t AMsk;
    uint32_t AMsk1;
    uint32_t AMsk2;
#elif defined(__NUC131__)
    uint32_t num;
    uint32_t type;
    uint32_t AMsk;
    uint32_t AMsk2;
    uint32_t AMsk3;
    uint32_t AMsk4;
#endif
} PinType;

typedef struct _PinDescription
{
#if defined(__M451__) | defined(__NANO100__) | defined(__NANO1X2__) |defined(__M032BT__)
    uint32_t MFP;
    uint32_t Mask;
    uint32_t Type;
#elif defined(__NUC240__)
    uint32_t MFP;
    uint32_t Mask;
    uint32_t ALTMsk;
    uint32_t ALT1Msk;
    uint32_t ALT2Msk;
    uint32_t Type;
#elif defined(__NUC131__)
    uint32_t MFP;
    uint32_t Mask;
    uint32_t ALTMsk;
    uint32_t ALT2Msk;
    uint32_t ALT3Msk;
    uint32_t ALT4Msk;
    uint32_t Type;
#endif
} PinDescription;

typedef struct _GPIOPinDescription
{
    GPIO_T *P;
    uint32_t bit;
    PinDescription Pin;
} GPIOPinDescription;

typedef struct _ADCPinDescription
{
#if defined(__M451__) || defined(__M480__) || defined(__M460__)
    EADC_T *A;
#elif defined(__NUC131__) | defined(__NUC240__) | defined(__NANO100__) | defined(__NANO1X2__) |defined(__M032BT__)
    ADC_T *A;
#endif
    uint32_t module;
    uint32_t ch;
    PinType pintype;
} ADCPinDescription;

typedef struct _PWMPinDescription
{
#if defined(__M032BT__)
    union {
        void *V;
        PWM_T  *P;
        BPWM_T *BP;
    };
#else
    PWM_T  *P;
#endif
    uint32_t module;
    IRQn_Type irq;
    uint32_t ch;
    uint32_t freq;
    PinType pintype;
} PWMPinDescription;

typedef struct _SPIPinDescription
{
    union {
        void *V;
        SPI_T *S;
#if defined(__M032BT__)
        USPI_T *US;
#endif
    };

    uint32_t module;
    IRQn_Type irq;
    uint32_t clksel;
    PinType pintype[4];
} SPIPinDescription;

typedef struct _UARTPinDescription
{
    UART_T *U;
    uint32_t module;
    IRQn_Type irq;
    PinType pintype[2];
} UARTPinDescription;

#if defined(__M451__) | defined(__NUC240__) | defined(__NUC131__) 
typedef struct _CANPinDescription
{
    CAN_T *C;
    uint32_t module;
    IRQn_Type irq;
    PinType pintype[2];
} CANPinDescription;
#endif

typedef struct _I2CPinDescription
{
    I2C_T *I;
    uint32_t module;
    PinType pintype[2];
} I2CPinDescription;


typedef struct _BoardToPin
{
    int32_t pin;
    uint8_t type;
    uint8_t num;
} BoardToPin;

/* Board definition */
#ifdef USE_BoardToPin
#define GPIO_TYPE   0
#define UART_TYPE   1
#define I2C_TYPE    2
#define SPI_TYPE    3
#define ADC_TYPE    4
#define PWM_TYPE    5
#define CAN_TYPE    6

#if defined(__NUC131__)
    #define BoardToPin_MAX_COUNT 44
#elif defined(__M032BT__)
    #define BoardToPin_MAX_COUNT 45
#elif defined(__NANO1X2__)
    #if defined(__IOT_EVB__)
        #define BoardToPin_MAX_COUNT 18
    #endif
#else
    #define BoardToPin_MAX_COUNT 81
#endif
    extern BoardToPin BoardToPinInfo[];
#endif

/*========== GPIO definition ==========*/
#if defined(__M451__)
#define GPIO_MAX_COUNT 101
extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#elif defined(__NUC240__)
#define GPIO_MAX_COUNT 101
extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) \
 do { \
     outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type); \
     outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP)  & ~Desc.Pin.ALTMsk));  \
     outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~Desc.Pin.ALT1Msk)); \
     outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~Desc.Pin.ALT2Msk)); \
 }while(0);
#elif defined(__NANO100__)
#define GPIO_MAX_COUNT 128
extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#elif defined(__NUC131__)
#define GPIO_MAX_COUNT 64
extern const GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) \
 do { \
     outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type); \
     outp32(&SYS->ALT_MFP, (inp32(&SYS->ALT_MFP)  & ~Desc.Pin.ALTMsk));  \
     outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~Desc.Pin.ALT2Msk)); \
     outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~Desc.Pin.ALT3Msk)); \
     outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~Desc.Pin.ALT4Msk)); \
 }while(0);

#elif defined(__M032BT__)
    #define GPIO_MAX_COUNT 56 /* real has 68 pin, only 56 to save memory space */
    extern const GPIOPinDescription GPIO_Desc[];
    #define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)

#elif defined(__NANO1X2__)
#if defined(__NANO102SC2AN__)
    #define GPIO_MAX_COUNT 64
#elif defined(__NANO102ZC2AN__)
    #define GPIO_MAX_COUNT 32
#elif defined(__NANO112__)
    #define GPIO_MAX_COUNT 100
#endif

extern GPIOPinDescription GPIO_Desc[];
#define GPIO_Config(Desc) outp32(Desc.Pin.MFP,(inp32(Desc.Pin.MFP) & ~Desc.Pin.Mask) | Desc.Pin.Type)
#endif


/*========== PWM definition ==========*/
#if defined(__M451__)
#define PWM_MAX_COUNT 8
extern PWMPinDescription PWM_Desc[];
#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

#elif defined(__NUC240__)

#define PWM_MAX_COUNT 8
extern PWMPinDescription PWM_Desc[];
//#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define PWM_Config(Desc) \
 do{ \
     outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
         outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk!=NULL) \
         outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk) | Desc.pintype.AMsk1); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
         outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
 }while(0);

#elif defined(__NANO100__)

#define PWM_MAX_COUNT 8
extern PWMPinDescription PWM_Desc[];
#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NANO1X2__)
#if defined(__IOT_EVB__)
    #define PWM_MAX_COUNT 1
#endif
extern PWMPinDescription PWM_Desc[];
#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

#elif defined(__NUC131__)
#define PWM_MAX_COUNT 10
extern PWMPinDescription PWM_Desc[];
//#define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define PWM_Config(Desc) \
do{ \
    outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
        outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
        outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk!=NULL) \
        outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk!=NULL) \
        outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4); \
}while(0);

#elif defined(__M032BT__)
    #define PWM_MAX_COUNT 12
    extern PWMPinDescription PWM_Desc[];
    #define PWM_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#endif

/*========== ADC definition ==========*/
#if defined(__M451__)
#define ADC_MAX_COUNT 11
extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NUC240__)
#define ADC_MAX_COUNT 8
extern ADCPinDescription ADC_Desc[];
//#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define ADC_Config(Desc) \
 do { \
     outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
         outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk!=NULL) \
         outp32(&SYS->ALT_MFP1,(inp32(&SYS->ALT_MFP1) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT1Msk) | Desc.pintype.AMsk1); \
     if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
         outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
 }while(0);
#elif defined(__NANO100__)
#define ADC_MAX_COUNT 10
extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NANO1X2__)
#if defined(__IOT_EVB__)
    #define ADC_MAX_COUNT 2
#endif
extern ADCPinDescription ADC_Desc[];
#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#elif defined(__NUC131__)
#define ADC_MAX_COUNT 6
extern ADCPinDescription ADC_Desc[];
//#define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);
#define ADC_Config(Desc) \
do { \
    outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALTMsk!=NULL) \
        outp32(&SYS->ALT_MFP,(inp32(&SYS->ALT_MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.ALTMsk) | Desc.pintype.AMsk); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk!=NULL) \
        outp32(&SYS->ALT_MFP2,(inp32(&SYS->ALT_MFP2) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT2Msk) | Desc.pintype.AMsk2); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk!=NULL) \
        outp32(&SYS->ALT_MFP3,(inp32(&SYS->ALT_MFP3) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT3Msk) | Desc.pintype.AMsk3); \
    if(GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk!=NULL) \
        outp32(&SYS->ALT_MFP4,(inp32(&SYS->ALT_MFP4) & ~GPIO_Desc[Desc.pintype.num].Pin.ALT4Msk) | Desc.pintype.AMsk4); \
}while(0);

#elif defined(__M032BT__)
    #define ADC_MAX_COUNT 6
    extern ADCPinDescription ADC_Desc[];
    #define ADC_Config(Desc) outp32(GPIO_Desc[Desc.pintype.num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype.num].Pin.MFP) & ~GPIO_Desc[Desc.pintype.num].Pin.Mask) | Desc.pintype.type);

#endif

/*========== SPI definition ==========*/
#if defined(__M451__)
    #define SPI_MAX_COUNT 1
    #define SPI_CHANNELS_NUM 1
#elif defined(__NUC240__)
    #define SPI_MAX_COUNT 1
    #define SPI_CHANNELS_NUM 1
#elif defined(__NANO100__)
    #define SPI_MAX_COUNT 3
#elif defined(__NANO1X2__)
    #if defined(__IOT_EVB__)
        #define SPI_MAX_COUNT 0
    #endif
#elif defined(__NUC131__)
    #define SPI_MAX_COUNT 1
    #define SPI_CHANNELS_NUM 1

#elif defined(__M032BT__)
    #define SPI_MAX_COUNT 2
    #define SPI_CHANNELS_NUM 2
#endif

extern SPIPinDescription SPI_Desc[];
#define SPI_SCK   0
// #define SPI_MOSI  1 //defined but not used
// #define SPI_MISO  2 //defined but not used
#define SPI_SS    3
#define SPI_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<4;i++) \
     if(Desc.pintype[i].num != 0) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);

/*========== UART definition ==========*/
#if defined(__M451__)
    #define UART_MAX_COUNT 4
#elif defined(__NUC240__)
    #define UART_MAX_COUNT 3
#elif defined(__NANO100__)
    #define UART_MAX_COUNT 2
#elif defined(__NANO1X2__)
    #if defined(__IOT_EVB__)
        #define UART_MAX_COUNT 2
    #endif
#elif defined(__NUC131__)
    #define UART_MAX_COUNT 1
#elif defined(__M032BT__)
    #define UART_MAX_COUNT 1
#endif

extern UARTPinDescription UART_Desc[];
#define UART_RX 0
#define UART_TX 1
#define UART_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);


/*========== CAN definition ==========*/
#if defined(__M451__) | defined(__NUC240__)| defined(__NUC131__)
#define CAN_MAX_COUNT 1
extern CANPinDescription CAN_Desc[];
#define CAN_RX 0
#define CAN_TX 1
#define CAN_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);
#endif

/*========== I2C definition ==========*/
#if defined(__M451__) | defined(__NUC240__) | defined(__NANO100__)
    #define I2C_MAX_COUNT 2
#elif defined(__NANO1X2__)
    #if defined(__IOT_EVB__)
        #define I2C_MAX_COUNT 1
    #endif
#elif defined(__NUC131__)
    #define I2C_MAX_COUNT 1
#elif defined(__M032BT__)
    #define I2C_MAX_COUNT 2
#endif

extern I2CPinDescription I2C_Desc[];
#define I2C_SDA 0
#define I2C_SCL 1
#define I2C_Config(Desc) \
do { \
    uint8_t i; \
    for(i=0;i<2;i++) \
        outp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP,(inp32(GPIO_Desc[Desc.pintype[i].num].Pin.MFP) & ~GPIO_Desc[Desc.pintype[i].num].Pin.Mask) | Desc.pintype[i].type); \
}while(0);


/*========== Arduino PIN mapping definition ==========*/
#if defined(__M451__)
    static const uint8_t SS   = 43;
    static const uint8_t MOSI = 42;
    static const uint8_t MISO = 40;
    static const uint8_t SCK  = 41;

    static const uint8_t SDA = 14;
    static const uint8_t SCL = 15;

    static const uint8_t A0  = 0;
    static const uint8_t A1  = 1;
    static const uint8_t A2  = 2;
    static const uint8_t A3  = 3;
    static const uint8_t A4  = 4;
    static const uint8_t A5  = 5;
    static const uint8_t A6  = 6;
    static const uint8_t A7  = 7;
    static const uint8_t A8  = 8;
    static const uint8_t A9  = 9;
    //static const uint8_t A10 = 10;
    //static const uint8_t A11 = 11;

#elif defined(__NUC240__)
    static const uint8_t SS   = 43;
    static const uint8_t MOSI = 42;
    static const uint8_t MISO = 40;
    static const uint8_t SCK  = 41;

    static const uint8_t SDA = 14;
    static const uint8_t SCL = 15;

    static const uint8_t A0 = 0;
    static const uint8_t A1 = 1;
    static const uint8_t A2 = 2;
    static const uint8_t A3 = 3;
    static const uint8_t A4 = 4;
    static const uint8_t A5 = 5;
    static const uint8_t A6 = 6;
    static const uint8_t A7 = 7;
    //static const uint8_t A8 = 8;
    //static const uint8_t A9 = 9;

#elif defined(__NANO100__)
    static const uint8_t SS   = 43;
    static const uint8_t MOSI = 42;
    static const uint8_t MISO = 40;
    static const uint8_t SCK  = 41;

    static const uint8_t SDA = 14;
    static const uint8_t SCL = 15;

    static const uint8_t A0 = 0;
    static const uint8_t A1 = 1;
    static const uint8_t A2 = 2;
    static const uint8_t A3 = 3;
    static const uint8_t A4 = 4;
    static const uint8_t A5 = 5;
    static const uint8_t A6 = 6;
    static const uint8_t A7 = 7;
    static const uint8_t A8 = 8;
    static const uint8_t A9 = 9;
#elif defined(__NUC131__)
    static const uint8_t SS   = 43;
    static const uint8_t MOSI = 42;
    static const uint8_t MISO = 40;
    static const uint8_t SCK  = 41;

    static const uint8_t LED_BUILTIN = 13;
    static const uint8_t SDA = 14;
    static const uint8_t SCL = 15;

    static const uint8_t A0 = 0;
    static const uint8_t A1 = 1;
    static const uint8_t A2 = 2;
    static const uint8_t A3 = 3;
    static const uint8_t A4 = 4;
    static const uint8_t A5 = 5;
    
#elif defined(__M032BT__)
    /* USE_BoardToPin Index */
    static const uint8_t SS   = 6;
    static const uint8_t MOSI = 8;
    static const uint8_t MISO = 7;
    static const uint8_t SCK  = 9;

    static const uint8_t LED_BUILTIN = 13;
    static const uint8_t SDA = 14;
    static const uint8_t SCL = 15;

    /* Analog Direct Access Index */
    static const uint8_t A0 = 0;
    static const uint8_t A1 = 1;
    static const uint8_t A2 = 2;
    static const uint8_t A3 = 3;
    static const uint8_t A4 = 4;
    static const uint8_t A5 = 5;

#elif defined(__NANO1X2__)
    #if defined(__IOT_EVB__)
        static const uint8_t A0 = 0;
        static const uint8_t A1 = 1;
        static const uint8_t SDA = 12;
        static const uint8_t SCL = 13;
    #endif
#endif

#endif  //end of defined(__M252__)

#endif