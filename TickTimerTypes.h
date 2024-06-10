#ifndef TICKTIMER_TYPES_H
#define TICKTIMER_TYPES_H


#include<stdint.h>

// Macro for TickTimer_TimerStruct initialization
#define TICK_TIMER_INIT(T) \
(T).startTs = 0; \
(T).value = 0; \
(T).inverseStartTs = (T).startTs ^ 0xffffu; \
(T).inverseValue = (T).value ^ 0xffffu; \

typedef struct TickTimer_TimerStruct
{
    uint16_t startTs;
    uint16_t value;
    uint16_t inverseStartTs;
    uint16_t inverseValue;
} TickTimer_TimerType;

typedef enum TickTimer_TimerStateEnum
{
    TickTimer_Uninitialized = 0u,
    TickTimer_Expired,
    TickTimer_Running
}TickTimer_TimerStateType;

typedef enum TickTimer_TimerModeEnum
{
    TickTimer_DelayMode = 0u,
    TickTimer_RateMode
}TickTimer_TimerModeType;

#endif