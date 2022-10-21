/* 
 * File:   CDC_Cfg.h
 * Author: tothpetiszilard
 *
 * Created on December 24, 2018, 3:05 PM
 */

#ifndef CDC_CFG_H
#define	CDC_CFG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../BMW_IBUS/IBUS.h"
#include "../SysTick/SysTick.h"
#include "../Bluetooth/Bluetooth.h"
#include "../MP3/MP3.h"

typedef struct _CDC_ToDo_tst
{
    uint8_t announcement_u1 :1;
    uint8_t pingResp_u1 :1;
    uint8_t statusFrame_u1 :1;
}CDC_ToDo_tst;

typedef enum _CDC_MediaSource_ten
{
    CDC_MEDIA_MP3 = 0u,
    CDC_MEDIA_BT = 1u,
    CDC_CONFIGMODE =2u
}CDC_MediaSource_ten;

typedef enum _CDC_StatusType_ten
{
    CDC_STOPPED,
    CDC_STARTING,
    CDC_STARTED,
    CDC_FASTFORWARD,
    CDC_FASTBACKWARD,
    CDC_STOPPING,
    CDC_SEEKING
}CDC_StatusType_ten;

#ifdef	__cplusplus
}
#endif

#endif	/* CDC_CFG_H */

