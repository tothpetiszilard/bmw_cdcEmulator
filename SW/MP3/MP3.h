/* 
 * File:   MP3.h
 * Author: tothpetiszilard
 *
 * Created on March 23, 2019, 12:17 PM
 */

#ifndef MP3_H
#define	MP3_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <xc.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum _MP3_CallbackType_ten
{
    MP3_CB_SETTRACK = 0u
    
}MP3_CallbackType_ten;
    
extern void MP3_Init(void);
extern void MP3_Cyclic(void);
extern void MP3_Pause(void);
extern void MP3_SelectTrack(uint8_t folder_u8,uint8_t track_u8);
extern void MP3_WakeUp(void);
extern void MP3_SaveTrack(void);
extern void MP3_ChangeMedia(void);
extern uint8_t MP3_GetMaxTracksPerFolder(void);
extern void MP3_SetMaxTracksPerFolder(uint8_t newValue_u8);

#ifdef	__cplusplus
}
#endif

#endif	/* MP3_H */

