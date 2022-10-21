/* 
 * File:   MP3_Cfg.h
 * Author: tothpetiszilard
 *
 * Created on March 23, 2019, 12:17 PM
 */

#ifndef MP3_CFG_H
#define	MP3_CFG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "MP3.h"
#include "../mcc_generated_files/eusart2.h"
#include "../BMW_CDC/CDC_Mp3_if.h"
#include "../SysTick/SysTick.h"
#include "../NvM/NvM.h"
    
#define MP3_RXBUFFER_SIZE   (8u)

#define MP3_TXBUFFER_SIZE   (10u)

#define MP3_STARTBYTE_U8    ((uint8_t)0x7Eu)
#define MP3_VERSION_U8      ((uint8_t)0xFFu)
#define MP3_CHKSUMSIZE_U8   ((uint8_t)2u)
#define MP3_ENDBYTE_u8      ((uint8_t)0xEFu)
    
#define MP3_HIGHBYTE(x)     ((uint8_t)(((uint16_t)x & (uint16_t)0xFF00u) >> 8u))
#define MP3_LOWBYTE(x)      ((uint8_t)x)
    
#define MP3_GETTIME()                       SysTick_Get()
    
#define MP3_CDC_CALLBACK(type_en,p1,p2)     CDC_MP3_Callback(type_en,p1,p2)
    
#define MP3_UART_INIT()                     EUSART2_Initialize()
#define MP3_UART_ISREADY()                  EUSART2_is_rx_ready()
#define MP3_UART_READ()                     EUSART2_Read()
#define MP3_UART_WRITE(data_u8)             EUSART2_Write(data_u8)

#define MP3_NVM_READ()                      NvM_Read()
#define MP3_NVM_READCFG()                   NvM_ReadCfg()
#define MP3_NVM_WRITE(data_u16)             NvM_Write(data_u16)
#define MP3_NVM_WRITECFG(cfgData_u8)        NvM_WriteCfg(cfgData_u8)
    
#ifdef	__cplusplus
}
#endif

#endif	/* MP3_CFG_H */

