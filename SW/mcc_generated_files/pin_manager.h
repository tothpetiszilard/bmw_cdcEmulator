/**
  @Generated Pin Manager Header File

  @Company:
    Microchip Technology Inc.

  @File Name:
    pin_manager.h

  @Summary:
    This is the Pin Manager file generated using PIC10 / PIC12 / PIC16 / PIC18 MCUs

  @Description
    This header file provides APIs for driver for .
    Generation Information :
        Product Revision  :  PIC10 / PIC12 / PIC16 / PIC18 MCUs - 1.76
        Device            :  PIC16F15325
        Driver Version    :  2.11
    The generated drivers are tested against the following:
        Compiler          :  XC8 2.00
        MPLAB 	          :  MPLAB X 5.10	
*/

/*
    (c) 2018 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
*/

#ifndef PIN_MANAGER_H
#define PIN_MANAGER_H

/**
  Section: Included Files
*/

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set BT_NXT aliases
#define BT_NXT_TRIS                 TRISAbits.TRISA0
#define BT_NXT_LAT                  LATAbits.LATA0
#define BT_NXT_PORT                 PORTAbits.RA0
#define BT_NXT_WPU                  WPUAbits.WPUA0
#define BT_NXT_OD                   ODCONAbits.ODCA0
#define BT_NXT_ANS                  ANSELAbits.ANSA0
#define BT_NXT_SetHigh()            do { LATAbits.LATA0 = 1; } while(0)
#define BT_NXT_SetLow()             do { LATAbits.LATA0 = 0; } while(0)
#define BT_NXT_Toggle()             do { LATAbits.LATA0 = ~LATAbits.LATA0; } while(0)
#define BT_NXT_GetValue()           PORTAbits.RA0
#define BT_NXT_SetDigitalInput()    do { TRISAbits.TRISA0 = 1; } while(0)
#define BT_NXT_SetDigitalOutput()   do { TRISAbits.TRISA0 = 0; } while(0)
#define BT_NXT_SetPullup()          do { WPUAbits.WPUA0 = 1; } while(0)
#define BT_NXT_ResetPullup()        do { WPUAbits.WPUA0 = 0; } while(0)
#define BT_NXT_SetPushPull()        do { ODCONAbits.ODCA0 = 0; } while(0)
#define BT_NXT_SetOpenDrain()       do { ODCONAbits.ODCA0 = 1; } while(0)
#define BT_NXT_SetAnalogMode()      do { ANSELAbits.ANSA0 = 1; } while(0)
#define BT_NXT_SetDigitalMode()     do { ANSELAbits.ANSA0 = 0; } while(0)

// get/set BT_PRV aliases
#define BT_PRV_TRIS                 TRISAbits.TRISA1
#define BT_PRV_LAT                  LATAbits.LATA1
#define BT_PRV_PORT                 PORTAbits.RA1
#define BT_PRV_WPU                  WPUAbits.WPUA1
#define BT_PRV_OD                   ODCONAbits.ODCA1
#define BT_PRV_ANS                  ANSELAbits.ANSA1
#define BT_PRV_SetHigh()            do { LATAbits.LATA1 = 1; } while(0)
#define BT_PRV_SetLow()             do { LATAbits.LATA1 = 0; } while(0)
#define BT_PRV_Toggle()             do { LATAbits.LATA1 = ~LATAbits.LATA1; } while(0)
#define BT_PRV_GetValue()           PORTAbits.RA1
#define BT_PRV_SetDigitalInput()    do { TRISAbits.TRISA1 = 1; } while(0)
#define BT_PRV_SetDigitalOutput()   do { TRISAbits.TRISA1 = 0; } while(0)
#define BT_PRV_SetPullup()          do { WPUAbits.WPUA1 = 1; } while(0)
#define BT_PRV_ResetPullup()        do { WPUAbits.WPUA1 = 0; } while(0)
#define BT_PRV_SetPushPull()        do { ODCONAbits.ODCA1 = 0; } while(0)
#define BT_PRV_SetOpenDrain()       do { ODCONAbits.ODCA1 = 1; } while(0)
#define BT_PRV_SetAnalogMode()      do { ANSELAbits.ANSA1 = 1; } while(0)
#define BT_PRV_SetDigitalMode()     do { ANSELAbits.ANSA1 = 0; } while(0)

// get/set BT_PLAY aliases
#define BT_PLAY_TRIS                 TRISAbits.TRISA2
#define BT_PLAY_LAT                  LATAbits.LATA2
#define BT_PLAY_PORT                 PORTAbits.RA2
#define BT_PLAY_WPU                  WPUAbits.WPUA2
#define BT_PLAY_OD                   ODCONAbits.ODCA2
#define BT_PLAY_ANS                  ANSELAbits.ANSA2
#define BT_PLAY_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define BT_PLAY_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define BT_PLAY_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define BT_PLAY_GetValue()           PORTAbits.RA2
#define BT_PLAY_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define BT_PLAY_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define BT_PLAY_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define BT_PLAY_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define BT_PLAY_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define BT_PLAY_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define BT_PLAY_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define BT_PLAY_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)

// get/set RA4 procedures
#define RA4_SetHigh()            do { LATAbits.LATA4 = 1; } while(0)
#define RA4_SetLow()             do { LATAbits.LATA4 = 0; } while(0)
#define RA4_Toggle()             do { LATAbits.LATA4 = ~LATAbits.LATA4; } while(0)
#define RA4_GetValue()              PORTAbits.RA4
#define RA4_SetDigitalInput()    do { TRISAbits.TRISA4 = 1; } while(0)
#define RA4_SetDigitalOutput()   do { TRISAbits.TRISA4 = 0; } while(0)
#define RA4_SetPullup()             do { WPUAbits.WPUA4 = 1; } while(0)
#define RA4_ResetPullup()           do { WPUAbits.WPUA4 = 0; } while(0)
#define RA4_SetAnalogMode()         do { ANSELAbits.ANSA4 = 1; } while(0)
#define RA4_SetDigitalMode()        do { ANSELAbits.ANSA4 = 0; } while(0)

// get/set BTEN aliases
#define BTEN_TRIS                 TRISAbits.TRISA5
#define BTEN_LAT                  LATAbits.LATA5
#define BTEN_PORT                 PORTAbits.RA5
#define BTEN_WPU                  WPUAbits.WPUA5
#define BTEN_OD                   ODCONAbits.ODCA5
#define BTEN_ANS                  ANSELAbits.ANSA5
#define BTEN_SetHigh()            do { LATAbits.LATA5 = 1; } while(0)
#define BTEN_SetLow()             do { LATAbits.LATA5 = 0; } while(0)
#define BTEN_Toggle()             do { LATAbits.LATA5 = ~LATAbits.LATA5; } while(0)
#define BTEN_GetValue()           PORTAbits.RA5
#define BTEN_SetDigitalInput()    do { TRISAbits.TRISA5 = 1; } while(0)
#define BTEN_SetDigitalOutput()   do { TRISAbits.TRISA5 = 0; } while(0)
#define BTEN_SetPullup()          do { WPUAbits.WPUA5 = 1; } while(0)
#define BTEN_ResetPullup()        do { WPUAbits.WPUA5 = 0; } while(0)
#define BTEN_SetPushPull()        do { ODCONAbits.ODCA5 = 0; } while(0)
#define BTEN_SetOpenDrain()       do { ODCONAbits.ODCA5 = 1; } while(0)
#define BTEN_SetAnalogMode()      do { ANSELAbits.ANSA5 = 1; } while(0)
#define BTEN_SetDigitalMode()     do { ANSELAbits.ANSA5 = 0; } while(0)

// get/set RC0 procedures
#define RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define RC0_GetValue()              PORTCbits.RC0
#define RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define RC0_SetPullup()             do { WPUCbits.WPUC0 = 1; } while(0)
#define RC0_ResetPullup()           do { WPUCbits.WPUC0 = 0; } while(0)
#define RC0_SetAnalogMode()         do { ANSELCbits.ANSC0 = 1; } while(0)
#define RC0_SetDigitalMode()        do { ANSELCbits.ANSC0 = 0; } while(0)

// get/set RC1 procedures
#define RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define RC1_GetValue()              PORTCbits.RC1
#define RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define RC1_SetPullup()             do { WPUCbits.WPUC1 = 1; } while(0)
#define RC1_ResetPullup()           do { WPUCbits.WPUC1 = 0; } while(0)
#define RC1_SetAnalogMode()         do { ANSELCbits.ANSC1 = 1; } while(0)
#define RC1_SetDigitalMode()        do { ANSELCbits.ANSC1 = 0; } while(0)

// get/set LINEN aliases
#define LINEN_TRIS                 TRISCbits.TRISC2
#define LINEN_LAT                  LATCbits.LATC2
#define LINEN_PORT                 PORTCbits.RC2
#define LINEN_WPU                  WPUCbits.WPUC2
#define LINEN_OD                   ODCONCbits.ODCC2
#define LINEN_ANS                  ANSELCbits.ANSC2
#define LINEN_SetHigh()            do { LATCbits.LATC2 = 1; } while(0)
#define LINEN_SetLow()             do { LATCbits.LATC2 = 0; } while(0)
#define LINEN_Toggle()             do { LATCbits.LATC2 = ~LATCbits.LATC2; } while(0)
#define LINEN_GetValue()           PORTCbits.RC2
#define LINEN_SetDigitalInput()    do { TRISCbits.TRISC2 = 1; } while(0)
#define LINEN_SetDigitalOutput()   do { TRISCbits.TRISC2 = 0; } while(0)
#define LINEN_SetPullup()          do { WPUCbits.WPUC2 = 1; } while(0)
#define LINEN_ResetPullup()        do { WPUCbits.WPUC2 = 0; } while(0)
#define LINEN_SetPushPull()        do { ODCONCbits.ODCC2 = 0; } while(0)
#define LINEN_SetOpenDrain()       do { ODCONCbits.ODCC2 = 1; } while(0)
#define LINEN_SetAnalogMode()      do { ANSELCbits.ANSC2 = 1; } while(0)
#define LINEN_SetDigitalMode()     do { ANSELCbits.ANSC2 = 0; } while(0)

// get/set IO_RC3 aliases
#define IO_RC3_TRIS                 TRISCbits.TRISC3
#define IO_RC3_LAT                  LATCbits.LATC3
#define IO_RC3_PORT                 PORTCbits.RC3
#define IO_RC3_WPU                  WPUCbits.WPUC3
#define IO_RC3_OD                   ODCONCbits.ODCC3
#define IO_RC3_ANS                  ANSELCbits.ANSC3
#define IO_RC3_SetHigh()            do { LATCbits.LATC3 = 1; } while(0)
#define IO_RC3_SetLow()             do { LATCbits.LATC3 = 0; } while(0)
#define IO_RC3_Toggle()             do { LATCbits.LATC3 = ~LATCbits.LATC3; } while(0)
#define IO_RC3_GetValue()           PORTCbits.RC3
#define IO_RC3_SetDigitalInput()    do { TRISCbits.TRISC3 = 1; } while(0)
#define IO_RC3_SetDigitalOutput()   do { TRISCbits.TRISC3 = 0; } while(0)
#define IO_RC3_SetPullup()          do { WPUCbits.WPUC3 = 1; } while(0)
#define IO_RC3_ResetPullup()        do { WPUCbits.WPUC3 = 0; } while(0)
#define IO_RC3_SetPushPull()        do { ODCONCbits.ODCC3 = 0; } while(0)
#define IO_RC3_SetOpenDrain()       do { ODCONCbits.ODCC3 = 1; } while(0)
#define IO_RC3_SetAnalogMode()      do { ANSELCbits.ANSC3 = 1; } while(0)
#define IO_RC3_SetDigitalMode()     do { ANSELCbits.ANSC3 = 0; } while(0)

// get/set RC5 procedures
#define RC5_SetHigh()            do { LATCbits.LATC5 = 1; } while(0)
#define RC5_SetLow()             do { LATCbits.LATC5 = 0; } while(0)
#define RC5_Toggle()             do { LATCbits.LATC5 = ~LATCbits.LATC5; } while(0)
#define RC5_GetValue()              PORTCbits.RC5
#define RC5_SetDigitalInput()    do { TRISCbits.TRISC5 = 1; } while(0)
#define RC5_SetDigitalOutput()   do { TRISCbits.TRISC5 = 0; } while(0)
#define RC5_SetPullup()             do { WPUCbits.WPUC5 = 1; } while(0)
#define RC5_ResetPullup()           do { WPUCbits.WPUC5 = 0; } while(0)
#define RC5_SetAnalogMode()         do { ANSELCbits.ANSC5 = 1; } while(0)
#define RC5_SetDigitalMode()        do { ANSELCbits.ANSC5 = 0; } while(0)

/**
   @Param
    none
   @Returns
    none
   @Description
    GPIO and peripheral I/O initialization
   @Example
    PIN_MANAGER_Initialize();
 */
void PIN_MANAGER_Initialize (void);

/**
 * @Param
    none
 * @Returns
    none
 * @Description
    Interrupt on Change Handling routine
 * @Example
    PIN_MANAGER_IOC();
 */
void PIN_MANAGER_IOC(void);



#endif // PIN_MANAGER_H
/**
 End of File
*/