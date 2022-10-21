/* 
 * File:   IBUS_Cfg.h
 * Author: tothpetiszilard
 *
 * Created on December 22, 2018, 1:35 PM
 */

#ifndef IBUS_CFG_H
#define	IBUS_CFG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../mcc_generated_files/eusart.h"
#include "../mcc_generated_files/pin_manager.h"
#include "../SysTick/SysTick.h"
    
typedef enum _IBUS_TxStatusType_ten
{
    IBUS_TX_IDLE = 0,
    IBUS_TX_REQ = 1u,
    IBUS_TX_TRANSMIT = 2u,
    IBUS_TX_VERIFY = 3u
}IBUS_TxStatusType_ten;

typedef enum _IBUS_RxStatusType_ten
{
    IBUS_RX_IDLE = 0,
    IBUS_RX_REC = 1u,
    IBUS_RX_READ = 2u
}IBUS_RxStatusType_ten;

#define IBUS_TXDATA_MAXSIZE     (20u)
#define IBUS_RXDATA_MAXSIZE     (100u)


#define IBUS_MYID_U8            ((uint8_t)0x18u)
#define IBUS_MINSIZE            (4u)

#ifdef	__cplusplus
}
#endif

#endif	/* IBUS_CFG_H */

