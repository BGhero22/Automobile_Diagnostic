#ifndef TICKTIMER_H
#define TICKTIMER_H



#include "TickTimerTypes.h"
#include <stdint.h>
#include <stdbool.h>

void TickTimer_Init(void);


void TickTimer_SetTimer(TickTimer_TimerType *timer, const uint16_t valueMs, TickTimer_TimerModeType mode);


void TickTimer_CheckTimer(TickTimer_TimerType *timer, TickTimer_TimerStateType *state);


void TickTimer_Tick(void);
#endif