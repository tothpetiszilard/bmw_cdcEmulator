#include "Bluetooth.h"
#include "Bluetooth_Cfg.h"

typedef enum _BT_ButtonState_ten
{
    BT_BUTTON_IDLE = 0u,
    BT_NEXT_PRESSED = 1u,
    BT_PREV_PRESSED = 2u,
    BT_PLAY_PRESSED = 3u
}BT_ButtonState_ten;

static BT_ButtonState_ten BT_buttonState_en = BT_BUTTON_IDLE;
static Bluetooth_StateType_ten BT_oldState_en = BT_DISCONNECTED;

static uint32_t BT_buttonTime_u32 = 0u;

static void BT_HandleButtons(void);

void Bluetooth_Cyclic(void)
{
    if ((BT_DISCONNECTED == BT_oldState_en) && (1u == BT_GETENSTATE()))
    {
        /* Bluetooth device connected */
        BT_oldState_en = BT_CONNECTED;
        CDC_BT_Callback(BT_CONNECTED);
    }
    else if ((BT_CONNECTED == BT_oldState_en) && (0u == BT_GETENSTATE()))
    {
        /* No bluetooth device connected */
        BT_oldState_en = BT_DISCONNECTED;
        CDC_BT_Callback(BT_DISCONNECTED);
    }
    else
    {
        /* No action needed */
    }
    BT_HandleButtons();
}

void Bluetooth_NextTrack(void)
{
    if ( BT_BUTTON_IDLE == BT_buttonState_en)
    {
        BT_PRESSNEXT();
        BT_buttonState_en = BT_NEXT_PRESSED;
        BT_buttonTime_u32 = BT_GETTIME() + BT_BUTTON_DELAY_U32;
    }
}

void Bluetooth_PrevTrack(void)
{
    if ( BT_BUTTON_IDLE == BT_buttonState_en)
    {
        BT_PRESSPREV();
        BT_buttonState_en = BT_PREV_PRESSED;
        BT_buttonTime_u32 = BT_GETTIME() + BT_BUTTON_DELAY_U32;
    }
}

void Bluetooth_PlayPause(void)
{
    if ( BT_BUTTON_IDLE == BT_buttonState_en)
    {
        BT_PRESSPLAY();
        BT_buttonState_en = BT_PLAY_PRESSED;
        BT_buttonTime_u32 = BT_GETTIME() + BT_BUTTON_DELAY_U32;
    }
}

static void BT_HandleButtons(void)
{
    if ((BT_BUTTON_IDLE != BT_buttonState_en) && (BT_buttonTime_u32 < BT_GETTIME()))
    {
        switch (BT_buttonState_en)
        {
            case BT_NEXT_PRESSED:
                BT_RELEASENEXT();
                break;
            case BT_PREV_PRESSED:
                BT_RELEASEPREV();
                break;
            case BT_PLAY_PRESSED:
                BT_RELEASEPLAY();
                break;
            default:
                break;
        }
        BT_buttonState_en = BT_BUTTON_IDLE;
    }
}