#include "CDC.h"
#include "CDC_Cfg.h"
#include "CDC_Mp3_if.h"

#include "CDC_Bluetooth_if.h"


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

static uint8_t CDC_configCnt_u8 = 0u;
static uint8_t CDC_mediaChngCnt_u8 = 0u;

static CDC_ToDo_tst CDC_ToDo_st;
static uint8_t * framePointer_pu8;
static CDC_StatusType_ten CDC_status_en;
static CDC_MediaSource_ten CDC_media_en;

uint8_t CDC_currentDiskId_u8 = 1u;
uint8_t CDC_currentTrackId_u8 = 1u;

static void HandleReceivedFrame(void);
static void SendStatusFrame(void);

void CDC_MP3_Callback(MP3_CallbackType_ten type_en,uint8_t param1, uint8_t param2)
{
    if ((CDC_MEDIA_MP3 == CDC_media_en) && (type_en == MP3_CB_SETTRACK))
    {
        CDC_currentDiskId_u8 = param1;
        CDC_currentTrackId_u8 = param2;
        CDC_ToDo_st.statusFrame_u1 = 1u;
        MP3_SaveTrack();
    }
}

void CDC_BT_Callback(Bluetooth_StateType_ten state_en)
{
    if (BT_CONNECTED == state_en)
    {
        CDC_media_en = CDC_MEDIA_BT;
        MP3_Pause();
        CDC_currentDiskId_u8 = 7u;
        CDC_currentTrackId_u8 = 1u;
        CDC_ToDo_st.statusFrame_u1 = 1u;
    }
    else
    {
        CDC_media_en = CDC_MEDIA_MP3;
        MP3_WakeUp();
        CDC_ToDo_st.statusFrame_u1 = 1u;
    }
}

void CDC_Init(void)
{
    CDC_ToDo_st.announcement_u1 = 1u;
    CDC_ToDo_st.pingResp_u1 = 0u;
    CDC_ToDo_st.statusFrame_u1 = 0u;
    CDC_status_en = CDC_STOPPED;
    CDC_media_en = CDC_MEDIA_MP3;
    CDC_mediaChngCnt_u8 = 0u;
    CDC_configCnt_u8 = 0u;
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
            MP3_SaveTrack();
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
            if (CDC_MEDIA_BT != CDC_media_en)
            {
                if ((1u == CDC_currentTrackId_u8 ) && (1u == framePointer_pu8[5u]) && (1u == CDC_currentDiskId_u8))
                {
                    if ( 3u > CDC_configCnt_u8)
                    {
                        CDC_configCnt_u8++;
                    }
                    else
                    {
                        CDC_currentDiskId_u8 = 8u;
                        CDC_media_en = CDC_CONFIGMODE;
                        CDC_status_en = CDC_STOPPED;
                        CDC_currentTrackId_u8 = MP3_GetMaxTracksPerFolder();
                    }
                }
                else
                {
                    if (8u == CDC_currentDiskId_u8)
                    {
                        MP3_SetMaxTracksPerFolder(CDC_currentTrackId_u8);
                        CDC_media_en = CDC_MEDIA_MP3;
                    }
                    CDC_configCnt_u8 = 0u;
                    CDC_currentTrackId_u8 = 1u;
                    CDC_currentDiskId_u8 = framePointer_pu8[5u];
                    CDC_status_en = CDC_STARTED;
                }
                
            }
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if (((framePointer_pu8[4u] == 0x0A) || (framePointer_pu8[4u] == 0x05)) && (framePointer_pu8[5u] == 0x00))
        {
            /* Next track */
            if (CDC_MEDIA_BT == CDC_media_en)
            {
                Bluetooth_NextTrack();
            }
            else if (CDC_MEDIA_MP3 == CDC_media_en)
            {
                if (CDC_currentTrackId_u8 < MP3_GetMaxTracksPerFolder())
                {
                    CDC_currentTrackId_u8++;
                    CDC_mediaChngCnt_u8 = 0u;
                }
            }
            else if (CDC_CONFIGMODE == CDC_media_en)
            {
                CDC_currentTrackId_u8++;
                CDC_mediaChngCnt_u8 = 0u;
            }
            else
            {
                
            }
            CDC_ToDo_st.statusFrame_u1 = 1u;
        }
        else if (((framePointer_pu8[4u] == 0x0A) || (framePointer_pu8[4u] == 0x05)) && (framePointer_pu8[5u] == 0x01))
        {
            /* Prev track */
            if (CDC_MEDIA_BT == CDC_media_en)
            {
                Bluetooth_PrevTrack();
            }
            else
            {
                if (CDC_currentTrackId_u8 > 1u)
                {
                    CDC_currentTrackId_u8--;
                    CDC_mediaChngCnt_u8 = 0u;
                }
                else
                {
                    if ((1u == CDC_currentTrackId_u8 ) && (1u == CDC_currentDiskId_u8))
                    {
                        if ( 3u > CDC_mediaChngCnt_u8)
                        {
                            CDC_mediaChngCnt_u8++;
                        }
                        else
                        {
                            MP3_ChangeMedia();
                            //CDC_status_en = CDC_STOPPED;
                            CDC_currentDiskId_u8 = 1u;
                            CDC_mediaChngCnt_u8 = 0u;
                        }
                    }
                }
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
        if ((CDC_STOPPED == CDC_status_en) && (CDC_MEDIA_MP3 == CDC_media_en))
        {
            MP3_Pause();
        }
        else if (CDC_MEDIA_MP3 == CDC_media_en)
        {
            MP3_SelectTrack(CDC_currentDiskId_u8,CDC_currentTrackId_u8);
        }
        else
        {
            /* BT active, no action needed */
        }
        CDC_ToDo_st.statusFrame_u1 = 0u;
    }
}