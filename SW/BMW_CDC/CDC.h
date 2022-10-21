/* 
 * File:   CDC.h
 * Author: tothpetiszilard
 *
 * Created on December 24, 2018, 3:05 PM
 */

#ifndef CDC_H
#define	CDC_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

extern void CDC_Init(void);
extern void CDC_Cyclic(void);
#ifdef	__cplusplus
}
#endif

#endif	/* CDC_H */

