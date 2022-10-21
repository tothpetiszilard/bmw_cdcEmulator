/* 
 * File:   CDC_Mp3_if.h
 * Author: tothpetiszilard
 *
 * Created on March 24, 2019, 1:37 PM
 */

#ifndef CDC_MP3_IF_H
#define	CDC_MP3_IF_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "../MP3/MP3.h"
    
extern void CDC_MP3_Callback(MP3_CallbackType_ten type_en,uint8_t param1, uint8_t param2);

#ifdef	__cplusplus
}
#endif

#endif	/* CDC_MP3_IF_H */

