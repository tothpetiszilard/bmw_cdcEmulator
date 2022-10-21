/* 
 * File:   NvM.h
 * Author: tothpetiszilard
 *
 * Created on March 30, 2019, 12:13 PM
 */

#ifndef NVM_H
#define	NVM_H

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif



extern void NvM_Init(void);
extern uint16_t NvM_Read(void);
extern void NvM_Write(uint16_t data_u16);
extern void NvM_WriteCfg(uint8_t cfgData_u8);
extern uint8_t NvM_ReadCfg(void);

#ifdef	__cplusplus
}
#endif

#endif	/* NVM_H */

