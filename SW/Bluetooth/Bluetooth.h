/* 
 * File:   Bluetooth.h
 * Author: tothpetiszilard
 *
 * Created on March 24, 2019, 2:47 PM
 */

#ifndef BLUETOOTH_H
#define	BLUETOOTH_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
    
    typedef enum _Bluetooth_StateType_ten
    {
        BT_DISCONNECTED = 0u,
        BT_CONNECTED = 1u
    }Bluetooth_StateType_ten;

    extern void Bluetooth_Cyclic(void);
    extern void Bluetooth_NextTrack(void);
    extern void Bluetooth_PrevTrack(void);
    extern void Bluetooth_PlayPause(void);

#ifdef	__cplusplus
}
#endif

#endif	/* BLUETOOTH_H */

