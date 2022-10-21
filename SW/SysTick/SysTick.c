#include "SysTick.h"
#include "SysTick_Cfg.h"

volatile uint32_t sysTick_u32 = 0u;

static void Timer_1msCallback(void);

static void Timer_1msCallback(void)
{
    sysTick_u32++;
}

void SysTick_Init(void)
{
    sysTick_u32 = 0u;
    TMR0_SetInterruptHandler(Timer_1msCallback);
    TMR0_StartTimer();
}

uint32_t SysTick_Get(void)
{
    return sysTick_u32;
}