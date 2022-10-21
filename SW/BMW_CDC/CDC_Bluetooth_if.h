/* 
 * File:   CDC_Bluetooth_if.h
 * Author: tothpetiszilard
 *
 * Created on March 24, 2019, 4:50 PM
 */

#ifndef CDC_BLUETOOTH_IF_H
#define	CDC_BLUETOOTH_IF_H

#ifdef	__cplusplus
extern "C" {
#endif
#include "../Bluetooth/Bluetooth.h"
    
    
extern void CDC_BT_Callback(Bluetooth_StateType_ten state_en);

#ifdef	__cplusplus
}
#endif

#endif	/* CDC_BLUETOOTH_IF_H */

