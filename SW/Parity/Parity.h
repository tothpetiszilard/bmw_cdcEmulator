/* 
 * File:   Parity.h
 * Author: tothpetiszilard
 *
 * Created on December 20, 2018, 1:27 PM
 */

#ifndef PARITY_H
#define	PARITY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>
    
extern uint8_t Parity_GetParityBit(uint8_t * data_pu8);

#ifdef	__cplusplus
}
#endif

#endif	/* PARITY_H */

