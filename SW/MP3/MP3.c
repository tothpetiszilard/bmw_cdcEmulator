#include "MP3.h"
#include "MP3_Cfg.h"

typedef enum _MP3_SrcType_ten
{
    SRC_NOMEDIA = 0u,
    SRC_USB = 1u,
    SRC_SDCARD = 2u
    
}MP3_SrcType_ten;

typedef struct _MP3_ToDoType_tst
{
    uint8_t init_u1 :1;
    uint8_t changeMedia_u1 :1;
    uint8_t getFilesCnt_u1 :1;
    uint8_t start_u1 :1;
    uint8_t pause_u1 :1;
    uint8_t changeTrack_u1 :1;
    uint8_t getTrack_u1 :1;
    uint8_t readNvm_u1 :1;
    uint8_t stop_u1 :1;
}MP3_ToDoType_tst;

static MP3_SrcType_ten currentSrc_en = SRC_NOMEDIA;
static MP3_ToDoType_tst MP3_toDo_st;
static uint8_t MP3_rxBuffer_au8[MP3_RXBUFFER_SIZE];
static uint8_t frameReceived_u8 = 0u;
static uint8_t MP3_txBuffer_au8[MP3_TXBUFFER_SIZE];
static uint16_t currTrack_u16 = 1;
static uint16_t specifiedTrack_u16 = 1u;
static uint16_t maxTrack_u16 = 1;

static uint8_t MP3_MaxTrackInFolder_u8 = 99u;

static void MP3_ProcessFrame(void);
static void MP3_SendCommand(uint8_t cmd_u8, uint8_t para1_u8, uint8_t para2_u8);
static void MP3_Receive(void);
static void MP3_Transmit(void);
static inline void MP3_DecodeRx(void);

void MP3_Init(void)
{
    MP3_UART_INIT();
    frameReceived_u8 = 0u;
    
    MP3_toDo_st.init_u1 = 1u;
    MP3_toDo_st.readNvm_u1 = 1u;
    
}

void MP3_Cyclic(void)
{
    if (0 < frameReceived_u8)
    {
        MP3_ProcessFrame();
    }
    MP3_Receive();
    MP3_Transmit();

}

void MP3_WakeUp(void)
{
   MP3_toDo_st.start_u1 = 1u;
   MP3_toDo_st.getTrack_u1 = 1u;
}

void MP3_SelectTrack(uint8_t folder_u8,uint8_t track_u8)
{
    if (1u < folder_u8)
    {
        specifiedTrack_u16 = ((uint16_t)(folder_u8-1u) * (MP3_MaxTrackInFolder_u8)) + (track_u8);
    }
    else
    {
        specifiedTrack_u16 = (uint16_t)(track_u8);
    }
    
    if (currTrack_u16 != specifiedTrack_u16)
    {
        /* Specify track */
        MP3_toDo_st.changeTrack_u1 = 1u;
    }
    else
    {
        MP3_toDo_st.start_u1 = 1u;
    }
    
}

void MP3_SaveTrack(void)
{
    if (SRC_USB == currentSrc_en)
    {
        MP3_NVM_WRITE(0x1000u | currTrack_u16);
    }
    else
    {
        MP3_NVM_WRITE(currTrack_u16);
    }
}

void MP3_SetMaxTracksPerFolder(uint8_t newValue_u8)
{
    MP3_MaxTrackInFolder_u8 = newValue_u8;
    MP3_NVM_WRITECFG(newValue_u8);
}

uint8_t MP3_GetMaxTracksPerFolder(void)
{
    return MP3_MaxTrackInFolder_u8;
}

void MP3_ChangeMedia(void)
{
    if (SRC_SDCARD == currentSrc_en)
    {
        currentSrc_en = SRC_USB;
    }
    else
    {
        currentSrc_en = SRC_SDCARD;
    }
    MP3_toDo_st.stop_u1 = 1u;
    specifiedTrack_u16 = 1u;
    MP3_toDo_st.changeMedia_u1 = 1u;
}

void MP3_Pause(void)
{
    MP3_toDo_st.pause_u1 = 1u;
}

static void MP3_Transmit(void)
{
    static uint32_t transmitTime_u32 = 1000u;
    uint32_t currTime_u32 = MP3_GETTIME();
    if (transmitTime_u32 < currTime_u32)
    {
        if (MP3_toDo_st.readNvm_u1 == 1u)
        {
            
            currTrack_u16 = MP3_NVM_READ();
            if (0 != (currTrack_u16 & 0x1000u))
            {
                currentSrc_en = SRC_USB;
                MP3_toDo_st.changeMedia_u1 = 1u;
                currTrack_u16 = (currTrack_u16 & 0x0FFFu);
            }
            else
            {
                currentSrc_en = SRC_SDCARD;
                MP3_toDo_st.changeMedia_u1 = 1u;
            }
            specifiedTrack_u16 = currTrack_u16;
            maxTrack_u16 = currTrack_u16;
            MP3_MaxTrackInFolder_u8 = MP3_NVM_READCFG();
            MP3_toDo_st.readNvm_u1 = 0u;
            MP3_toDo_st.getFilesCnt_u1 = 1u;
            MP3_toDo_st.changeTrack_u1 = 1u;
        }
        else if (MP3_toDo_st.init_u1 == 1u)
        {
            MP3_SendCommand(0x16u,0x00u,0x00u); /* Stop command */
            transmitTime_u32 = currTime_u32 + 2000u;
            MP3_toDo_st.init_u1 = 0u;
        }
        else if (MP3_toDo_st.stop_u1 == 1)
        {
            MP3_SendCommand(0x16u,0x00u,0x00u); /* Stop command */
            transmitTime_u32 = currTime_u32 + 200u;
            MP3_toDo_st.stop_u1 = 0u;
        }
        else if (MP3_toDo_st.changeMedia_u1 == 1)
        {
            MP3_SendCommand(0x09u,0x00u,(uint8_t)currentSrc_en); /* Change media */
            transmitTime_u32 = currTime_u32 + 200u;
            MP3_toDo_st.changeMedia_u1 = 0u;
            MP3_toDo_st.getFilesCnt_u1 = 1u;
            MP3_toDo_st.changeTrack_u1 = 1u;
        }
        else if (MP3_toDo_st.getFilesCnt_u1 == 1)
        {
            if (currentSrc_en == SRC_SDCARD)
            {
                MP3_SendCommand(0x48u,0x00u,0x00u); /* Get file count on SD */
            }
            else
            {
                MP3_SendCommand(0x47u,0x00u,0x00u); /* Get file count on SD */
            }
            transmitTime_u32 = currTime_u32 + 200u;
        }
        else if (MP3_toDo_st.changeTrack_u1 == 1)
        {
            MP3_SendCommand(0x03u,MP3_HIGHBYTE(specifiedTrack_u16),MP3_LOWBYTE(specifiedTrack_u16));
            transmitTime_u32 = currTime_u32 + 200u;
            MP3_toDo_st.changeTrack_u1 = 0u;
            MP3_toDo_st.getTrack_u1 = 1u;
        }        
        else if (MP3_toDo_st.getTrack_u1 == 1)
        {
            if (currentSrc_en == SRC_SDCARD)
            {
                MP3_SendCommand(0x4Cu,0x00,0x00); /* Get current track on SD */
            }
            else
            {
                MP3_SendCommand(0x4Bu,0x00,0x00); /* Get current track on USB */
            }
            transmitTime_u32 = currTime_u32 + 200u;
        }
        else if (MP3_toDo_st.start_u1 == 1u)
        {
            MP3_SendCommand(0x0Du,0x00u,0x00); /* Play command */
            transmitTime_u32 = currTime_u32 + 200u;
            MP3_toDo_st.start_u1 = 0u;
        }
        else if (MP3_toDo_st.pause_u1 == 1u)
        {
            MP3_SendCommand(0x0Eu,0x00u,0x00); /* Pause command */
            transmitTime_u32 = currTime_u32 + 200u;
            MP3_toDo_st.pause_u1 = 0u;
        }
        else
        {
            /* Nothing to do, sleep */
        }
    }
}

static void MP3_SendCommand(uint8_t cmd_u8, uint8_t para1_u8, uint8_t para2_u8)
{
    uint8_t tmp_u8 = 0u;
    uint16_t checksum_u16 =0u;
    MP3_txBuffer_au8[0u] = MP3_STARTBYTE_U8;
    MP3_txBuffer_au8[1u] = MP3_VERSION_U8;
    MP3_txBuffer_au8[2u] = 6u;
    MP3_txBuffer_au8[3u] = cmd_u8;
    MP3_txBuffer_au8[4u] = 1u; /* No feedback needed */
    MP3_txBuffer_au8[5u] = para1_u8;
    MP3_txBuffer_au8[6u] = para2_u8;
    MP3_txBuffer_au8[9u] = MP3_ENDBYTE_u8;
    for (tmp_u8 = 0; tmp_u8 < 6u; tmp_u8++)
    {
        checksum_u16 -= (uint16_t)MP3_txBuffer_au8[1u+tmp_u8];
    }
    MP3_txBuffer_au8[7] = MP3_HIGHBYTE(checksum_u16);
    MP3_txBuffer_au8[8] = MP3_LOWBYTE(checksum_u16);
    for (tmp_u8 = 0; tmp_u8 < MP3_TXBUFFER_SIZE; tmp_u8++)
    {
        MP3_UART_WRITE(MP3_txBuffer_au8[tmp_u8]);
    }
}

static void MP3_ProcessFrame(void)
{
    uint8_t tmp_u8 = 0u;
    uint16_t chksum_u16 = 0u;
    if ((MP3_VERSION_U8 == MP3_rxBuffer_au8[0u]) && (MP3_CHKSUMSIZE_U8 < MP3_rxBuffer_au8[1u]))
    {
        for (tmp_u8 = 0u; tmp_u8 < MP3_rxBuffer_au8[1u]; tmp_u8++ )
        {
            chksum_u16 -= MP3_rxBuffer_au8[tmp_u8];
        }
        if ((MP3_HIGHBYTE(chksum_u16) == MP3_rxBuffer_au8[6u]) && (MP3_LOWBYTE(chksum_u16) == MP3_rxBuffer_au8[7u]))
        {
            /* Checksum correct */
            MP3_DecodeRx();
        }
    }
    frameReceived_u8 = 0u;
}

static inline void MP3_DecodeRx(void)
{
    uint8_t calcTrack_u8 = 1u;
    uint8_t calcDisk_u8 = 1u;
    switch(MP3_rxBuffer_au8[2u])
    {
        case 0x41:
            /* ACK */
            break;
        case 0x47: /* Files count */ 
        case 0x48:
            maxTrack_u16 = (uint16_t)MP3_rxBuffer_au8[4u] <<8u;
            maxTrack_u16 |= (uint16_t)MP3_rxBuffer_au8[5u];
            if (0u != maxTrack_u16)
            {
                MP3_toDo_st.getFilesCnt_u1 = 0u;
                if (0x47u == MP3_rxBuffer_au8[2u])
                {
                    currentSrc_en = SRC_USB;
                }
                else if (0x48u == MP3_rxBuffer_au8[2u])
                {
                    currentSrc_en = SRC_SDCARD;
                }
            }
            else
            {
                if (SRC_SDCARD == currentSrc_en)
                {
                    currentSrc_en = SRC_USB;
                }
                else
                {
                    currentSrc_en = SRC_SDCARD;
                }
            }
            break;
        case 0x4Bu:
        case 0x4Cu:
        case 0x4Du:
            /* Current track */
            currTrack_u16 = (uint16_t)MP3_rxBuffer_au8[4u] << 8u;
            currTrack_u16 |= (uint16_t)MP3_rxBuffer_au8[5u];
            if (currTrack_u16 != 0)
            {
                MP3_toDo_st.getTrack_u1 = 0u;
                calcTrack_u8 = (currTrack_u16 % (MP3_MaxTrackInFolder_u8));
                if (0 == calcTrack_u8)
                {
                    calcTrack_u8 = MP3_MaxTrackInFolder_u8;
                    calcDisk_u8 = (currTrack_u16 / MP3_MaxTrackInFolder_u8);
                }
                else
                {
                    calcDisk_u8 = (1u+(currTrack_u16 / (MP3_MaxTrackInFolder_u8)));
                }
                MP3_CDC_CALLBACK(MP3_CB_SETTRACK,calcDisk_u8,calcTrack_u8);
            }
            else
            {
                MP3_toDo_st.getFilesCnt_u1 = 1u;
                MP3_toDo_st.changeTrack_u1 = 1u;
                if (0x4Cu == MP3_rxBuffer_au8[2u])
                {
                    currentSrc_en = SRC_USB;
                }
                else if (0x4Bu == MP3_rxBuffer_au8[2u])
                {
                    currentSrc_en = SRC_SDCARD;
                }
            }
            
            break;
        case 0x3A:
            if ((0u == MP3_rxBuffer_au8[4u]) && (1u == MP3_rxBuffer_au8[5u]))
            {
                /* USB plugged in */
                currentSrc_en = SRC_USB;
                MP3_toDo_st.getFilesCnt_u1 = 1u;
                specifiedTrack_u16 = 1u;
                MP3_toDo_st.changeTrack_u1 = 1u;
            }
            else if ((0u == MP3_rxBuffer_au8[4u]) && (2u == MP3_rxBuffer_au8[5u]))
            {
                /* SD plugged in */
                currentSrc_en = SRC_SDCARD;
                MP3_toDo_st.getFilesCnt_u1 = 1u;
                specifiedTrack_u16 = 1u;
                MP3_toDo_st.changeTrack_u1 = 1u;
            }
            else
            {
                /* Unknown media */
            }
            break;
        case 0x3B:
            if ((0u == MP3_rxBuffer_au8[4u]) && (1u == MP3_rxBuffer_au8[5u]))
            {
                /* USB disconnected */
                currentSrc_en = SRC_SDCARD;
                MP3_toDo_st.getTrack_u1 = 1u;
            }
            else if ((0u == MP3_rxBuffer_au8[4u]) && (2u == MP3_rxBuffer_au8[5u]))
            {
                /* SD disconnected */
                currentSrc_en = SRC_USB;
                MP3_toDo_st.getTrack_u1 = 1u;
            }
            else
            {
                /* Unknown media */
            }
            break;
        case 0x3Cu:
        case 0x3Du:
        case 0x3Eu:
            specifiedTrack_u16 = (uint16_t)MP3_rxBuffer_au8[4u] <<8u;
            specifiedTrack_u16 |= (uint16_t)MP3_rxBuffer_au8[5u];
            if (maxTrack_u16 > specifiedTrack_u16)
            {
                specifiedTrack_u16++;
            }
            else
            {
                specifiedTrack_u16 = 1u;
            }
            MP3_toDo_st.changeTrack_u1 = 1u;
            break;
        default:
            break;
    }
}

static void MP3_Receive(void)
{
    static uint8_t rxEnabled_u8= 0u;
    uint8_t tmp_u8 = 0u;
    if (0u == rxEnabled_u8)
    {
        if (0u < MP3_UART_ISREADY() && (MP3_STARTBYTE_U8 == MP3_UART_READ()))
        {
            rxEnabled_u8 = 1u;
        }
    }
    if (1u == rxEnabled_u8)
    {
        /* Start byte already received */
        if (MP3_RXBUFFER_SIZE < MP3_UART_ISREADY())
        {
            for (tmp_u8 = 0u; tmp_u8 < MP3_RXBUFFER_SIZE; tmp_u8++)
            {
                MP3_rxBuffer_au8[tmp_u8] = MP3_UART_READ();
            }
            if (MP3_ENDBYTE_u8 == MP3_UART_READ())
            {
                frameReceived_u8 = 1u;
            }
            rxEnabled_u8 = 0u;
        }
    }
}