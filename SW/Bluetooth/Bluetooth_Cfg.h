/* 
 * File:   Bluetooth_Cfg.h
 * Author: tothpetiszilard
 *
 * Created on March 24, 2019, 2:48 PM
 */

#ifndef BLUETOOTH_CFG_H
#define	BLUETOOTH_CFG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../mcc_generated_files/pin_manager.h"
#include "../SysTick/SysTick.h"
#include "../BMW_CDC/CDC_Bluetooth_if.h"

#define BT_BUTTON_DELAY_U32                 ((uint32_t)100u) /* in msec */
    
#define BT_PRESSNEXT()                      BT_NXT_SetHigh()
#define BT_RELEASENEXT()                    BT_NXT_SetLow()
#define BT_PRESSPREV()                      BT_PRV_SetHigh()
#define BT_RELEASEPREV()                    BT_PRV_SetLow()
#define BT_PRESSPLAY()                      BT_PLAY_SetHigh()
#define BT_RELEASEPLAY()                    BT_PLAY_SetLow()
    
#define BT_GETENSTATE()                     BTEN_GetValue()
#define BT_CDC_CALLBACK(state_en)           CDC_BT_Callback(state_en)
    
#define BT_GETTIME()                        SysTick_Get()
    
#ifdef	__cplusplus
}
#endif

#endif	/* BLUETOOTH_CFG_H */

