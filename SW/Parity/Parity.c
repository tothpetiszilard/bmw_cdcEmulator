#include "Parity.h"


uint8_t Parity_GetParityBit(uint8_t * data_pu8)
{
    uint8_t inData_u8;
    uint8_t onesCnt_u8 = 0u;
    uint8_t tmp_u8 = 0u;
    inData_u8 = *data_pu8;
    
    for (tmp_u8 = 0u; tmp_u8 < (uint8_t)8u; tmp_u8++)
    {
        if ((uint8_t)0x01u == (inData_u8 & (uint8_t)0x01u))
        {
            onesCnt_u8++;
        }
        inData_u8 = (uint8_t)(inData_u8 >> (uint8_t)1u);
    }
    return (uint8_t)(onesCnt_u8 % 2u);
}