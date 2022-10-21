#include "IBUS.h"
#include "IBUS_Cfg.h"

static uint8_t txBuffer_au8[IBUS_MINSIZE + IBUS_TXDATA_MAXSIZE];
static uint8_t rxBuffer_au8[IBUS_MINSIZE + IBUS_RXDATA_MAXSIZE];
static IBUS_TxStatusType_ten txStatus_en = IBUS_TX_IDLE;
static IBUS_RxStatusType_ten rxStatus_en = IBUS_RX_IDLE;
static uint32_t lastRxTime_u32;
static uint8_t CalcChecksum(uint8_t * inputData_au8, uint8_t length_u8);
static void IBUS_Transmit();
static void IBUS_Receive();

void IBUS_Init(void)
{
    uint8_t tmp_u8 = 0u;
    txStatus_en = IBUS_TX_IDLE;
    rxStatus_en = IBUS_RX_IDLE;
    for (tmp_u8 = 0u; tmp_u8 < (IBUS_MINSIZE + IBUS_TXDATA_MAXSIZE); tmp_u8++)
    {
        txBuffer_au8[tmp_u8] = 0u;
    }
    lastRxTime_u32 = SysTick_Get();
    EUSART_Initialize();
    IO_RC2_SetHigh();
}

void IBUS_Cyclic(void)
{
    IBUS_Receive();
    IBUS_Transmit();
}

static void IBUS_Receive()
{
    static uint8_t rxPos_u8 = 0;
    uint8_t data_u8 = 0u;
    if ((txStatus_en != IBUS_TX_VERIFY) && (0 < EUSART_is_rx_ready()))
    {
        
        switch(rxStatus_en)
        {
            case IBUS_RX_IDLE:
                if ((lastRxTime_u32 + 2) < SysTick_Get())
                {
                    /* New frame started by anyone else */
                    data_u8 = EUSART_Read();
                    if (IBUS_RADIOID_U8 == data_u8)
                    {
                        rxBuffer_au8[rxPos_u8++] = data_u8;
                        rxStatus_en = IBUS_RX_REC;
                    }
                }
                else
                {
                    /* Don't care frame is arriving */
                    EUSART_Read();
                }
                break;
            case IBUS_RX_REC:
            {
                if ((rxPos_u8 > 1) && (rxPos_u8 >= (1u + rxBuffer_au8[1])))
                {
                    rxBuffer_au8[rxPos_u8] = EUSART_Read();
                    /* Checksum arrived */
                    if (rxBuffer_au8[rxPos_u8] == CalcChecksum(rxBuffer_au8,rxPos_u8))
                    {
                        /* Checksum valid */
                        rxStatus_en = IBUS_RX_READ;
                    }
                    else
                    {
                        rxStatus_en = IBUS_RX_IDLE;
                    }
                    rxPos_u8 = 0u;
                }
                else if (rxPos_u8 == 2)
                {
                    data_u8 = EUSART_Read();
                    if (IBUS_MYID_U8 != data_u8)
                    {
                        rxBuffer_au8[rxPos_u8] = data_u8;
                        rxStatus_en = IBUS_RX_IDLE;
                        rxPos_u8 = 0u;
                    }
                    else
                    {
                        rxBuffer_au8[rxPos_u8++] = data_u8;
                    }
                }
                else
                {
                    rxBuffer_au8[rxPos_u8++] = EUSART_Read();
                }
                break;
            }
            default:
                EUSART_Read();
                break;
        }
        lastRxTime_u32 = SysTick_Get();
    }
}

static void IBUS_Transmit()
{
    static uint8_t lastSentByte_u8;
    static uint8_t txPos_u8 = 0;
    switch (txStatus_en)
    {
        case IBUS_TX_REQ:
            if ((rxStatus_en == IBUS_RX_IDLE) && ((lastRxTime_u32 + 4) <= SysTick_Get()))
            {
                txStatus_en = IBUS_TX_TRANSMIT;
            }    
            break;
        case IBUS_TX_TRANSMIT:
        {
            if (txPos_u8 < (txBuffer_au8[1u] + 2u))
            {
                lastSentByte_u8 = txBuffer_au8[txPos_u8];
                EUSART_Write(txBuffer_au8[txPos_u8++]);
                txStatus_en = IBUS_TX_VERIFY;
            }
            else
            {
                txPos_u8 = 0;
                txStatus_en = IBUS_TX_IDLE;
            }
            break;
        }
        case IBUS_TX_VERIFY:
        {
            if (lastSentByte_u8 == EUSART_Read())
            {
                txStatus_en = IBUS_TX_TRANSMIT;
            }
            else
            {
                /* Collision */
                lastRxTime_u32 = SysTick_Get();
                txPos_u8 = 0u; /* Restart */
                txStatus_en = IBUS_TX_REQ;
            }
            break;
        }
        default:
            break;
    }
    
}

uint8_t * IBUS_GetFrame(void)
{
    if (IBUS_RX_READ == rxStatus_en)
    {
        return rxBuffer_au8; 
    }
    else
    {
        return 0;
    }
}

void IBUS_ReadDone(void)
{
    rxStatus_en = IBUS_RX_IDLE;
}

uint8_t IBUS_SendFrame(uint8_t destDev_u8, uint8_t * data_au8, uint8_t length_u8)
{
    uint8_t tmp_u8 = 0;
    if ((IBUS_TXDATA_MAXSIZE >= length_u8) && (IBUS_TX_IDLE == txStatus_en))
    {
        txBuffer_au8[0] = IBUS_MYID_U8;
        txBuffer_au8[1] = 2u + length_u8; /* CRC + Dest dev + Data len */
        txBuffer_au8[2] = destDev_u8;
        for (tmp_u8 = 0u; tmp_u8 < length_u8; tmp_u8++)
        {
            txBuffer_au8[3u + tmp_u8] = data_au8[tmp_u8];
        }
        txBuffer_au8[3u + length_u8] = CalcChecksum(txBuffer_au8,(3u + length_u8));
        txStatus_en = IBUS_TX_REQ;
        return 0;
    }
    else
    {
        return 1;
    }
}

static uint8_t CalcChecksum(uint8_t * inputData_au8, uint8_t length_u8)
{
    uint8_t crc_u8 = 0u;
    for (uint8_t i = 0; i < length_u8; i++)
    {
        crc_u8 ^= inputData_au8[i];
    }
    return crc_u8;
}