/* 
 * File:   IBUS.h
 * Author: tothpetiszilard
 *
 * Created on December 22, 2018, 1:35 PM
 */

#ifndef IBUS_H
#define	IBUS_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#define IBUS_RADIOID_U8         ((uint8_t)0x68u)
#define IBUS_BROADCASTID_U8     ((uint8_t)0xFFu)
    
extern void IBUS_Init(void);
extern void IBUS_Cyclic(void);
extern uint8_t IBUS_SendFrame(uint8_t destDev_u8, uint8_t * data_au8, uint8_t length_u8);
extern uint8_t * IBUS_GetFrame(void);
extern void IBUS_ReadDone(void);

#ifdef	__cplusplus
}
#endif

#endif	/* IBUS_H */

