#include "CDC.h"
#include "CDC_Cfg.h"

const uint8_t cdcAnnouncement_cau8[2] = 
{
   0x02u,
   0x01u
};

const uint8_t cdcPingResp_cau8[2] = 
{
   0x02u,
   0x00u
};

static CDC_ToDo_tst CDC_ToDo_st;
static uint8_t * framePointer_pu8;
static CDC_StatusType_ten CDC_status_en;

uint8_t CDC_currentDiskId_u8 = 1;
uint8_t CDC_currentTrackId_u8 = 1;

static void HandleReceivedFrame(void);
static void SendStatusFrame(void);

void CDC_Init(void)
{
    CDC_ToDo_st.announcement_u1 = 1u;
    CDC_ToDo_st.pingResp_u1 = 0u;
    CDC_ToDo_st.statusFrame_u1 = 0u;
    CDC_status_en = CDC_STOPPED;
}

void CDC_Cyclic(void)
{
    static uint32_t announceTime_u32 = 0u;
    framePointer_pu8 = IBUS_GetFrame();
    if (0 != framePointer_pu8)
    {
        HandleReceivedFrame();
        IBUS_ReadDone();
    }
    else
    {
        if ((0 < CDC_ToDo_st.announcement_u1) && (announceTime_u32 < SysTick_Get()))
        {
            IBUS_SendFrame((uint8_t)IBUS_BROADCASTID_U8,(uint8_t*)cdcAnnouncement_cau8,2u);
            announceTime_u32 = (5000u + SysTick_Get());
        }
        else if (0 < CDC_ToDo_st.pingResp_u1)
        {
            if (0 == IBUS_SendFrame((uint8_t)IBUS_BROADCASTID_U8,(uint8_t*)cdcPingResp_cau8,2u))
            {
                CDC_ToDo_st.pingResp_u1 = 0;
            }
        }
        else if (0 < CDC_ToDo_st.statusFrame_u1)
        {
            SendStatusFrame();
        }
    }
}

static void HandleReceivedFrame(void)
{
    if ((framePointer_pu8[1u] == 3u) && (framePointer_pu8[3u] == 1u))
    {
        /* Ping received */
        CDC_ToDo_st.pingResp_u1 = 1u;
        CDC_ToDo_st.announcement_u1 = 0u;
    }
    else if ((framePointer_pu8[1u] == 5u) && (framePointer_pu8[3u] == 0x38))
    {
        if ((framePointer_pu8[4u] == 0x00) && (framePointer_pu8[5u] == 0x00))
        {
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if ((framePointer_pu8[4u] == 0x01) && (framePointer_pu8[5u] == 0x00))
        {
            /* Stop playing command */
            CDC_status_en = CDC_STOPPED;
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if ((framePointer_pu8[4u] == 0x03) && (framePointer_pu8[5u] == 0x00))
        {
            /* Start playing command */
            CDC_status_en = CDC_STARTED;
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if ((framePointer_pu8[4u] == 0x04) && (framePointer_pu8[5u] == 0x00))
        {
            /* Start playing command */
            CDC_status_en = CDC_FASTFORWARD;
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if ((framePointer_pu8[4u] == 0x04) && (framePointer_pu8[5u] == 0x01))
        {
            /* Start playing command */
            CDC_status_en = CDC_FASTBACKWARD;
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if (framePointer_pu8[4u] == 0x06)
        {
            /* Change disk */
            CDC_currentTrackId_u8 = 1u;
            CDC_currentDiskId_u8 = framePointer_pu8[5u];
            CDC_status_en = CDC_STARTED;
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if (((framePointer_pu8[4u] == 0x0A) || (framePointer_pu8[4u] == 0x05)) && (framePointer_pu8[5u] == 0x00))
        {
            /* Next track */
            if (CDC_currentTrackId_u8 < 99)
            {
                CDC_currentTrackId_u8++;
            }
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if (((framePointer_pu8[4u] == 0x0A) || (framePointer_pu8[4u] == 0x05)) && (framePointer_pu8[5u] == 0x01))
        {
            /* Prev track */
            if (CDC_currentTrackId_u8 > 1u)
            {
                CDC_currentTrackId_u8--;
            }
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else
        {
            /* Unsupported command */
        }
    }
    else
    {
        /* Unknown frame */
    }
}

static void SendStatusFrame(void)
{
    uint8_t buffer_au8[8u]; /* Data len*/
    buffer_au8[0] = 0x39;
    buffer_au8[3] = 0x00;
    buffer_au8[4] = 0x3F;
    buffer_au8[5] = 0x00;
    buffer_au8[6] = CDC_currentDiskId_u8;
    buffer_au8[7] = (uint8_t)(((CDC_currentTrackId_u8 / 10u) << 4u) | (CDC_currentTrackId_u8 % 10u));
    switch (CDC_status_en)
    {
        case CDC_STOPPED:
            buffer_au8[1] = 0x00;
            buffer_au8[2] = 0x02; // Stopped
            break;
        case CDC_STARTED:
            buffer_au8[1] = 0x00;
            buffer_au8[2] = 0x09; // Started
            break;
        case CDC_FASTFORWARD:
            buffer_au8[1] = 0x03;
            buffer_au8[2] = 0x09; // FF
            break;
        case CDC_FASTBACKWARD:
            buffer_au8[1] = 0x04;
            buffer_au8[2] = 0x09; // FB
            break;
        default:
            break;
    }
    if (0 == IBUS_SendFrame(IBUS_RADIOID_U8,buffer_au8,8u))
    {
        CDC_ToDo_st.statusFrame_u1 = 0u;
    }
}