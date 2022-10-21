/* 
 * File:   SysTick.h
 * Author: tothpetiszilard
 *
 * Created on December 24, 2018, 12:02 PM
 */

#ifndef SYSTICK_H
#define	SYSTICK_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <stdint.h>
    
void SysTick_Init(void);

uint32_t SysTick_Get(void);

#ifdef	__cplusplus
}
#endif

#endif	/* SYSTICK_H */

