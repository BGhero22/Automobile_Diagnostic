#include "TickTimer.h"

static bool TickTimer_initialized = false;
static uint16_t counter;

void TickTimer_Init(void)
{
    
    counter = 0;
    TickTimer_initialized = true;
   
}

void TickTimer_SetTimer(TickTimer_TimerType *timer, const uint16_t valueMs, TickTimer_TimerModeType mode)
{
  

   

    if (true == TickTimer_initialized)
    {
        switch (mode)
        {
        case TickTimer_DelayMode:
            timer->startTs = counter;
            timer->value = valueMs;
            
            break;

        case TickTimer_RateMode:
            if (((timer->startTs ^ timer->inverseStartTs) != 0xffffu) || ((timer->value ^ timer->inverseValue) != 0xffffu))
            {
                timer->startTs = counter;
            }
            else
            {
                timer->startTs += timer->value;
            }
            timer->value = valueMs;
           
            break;

        default:
            break;
        }

        timer->inverseStartTs = timer->startTs ^ 0xffffu;
        timer->inverseValue = timer->value ^ 0xffffu;
    }

   
}

void TickTimer_CheckTimer(TickTimer_TimerType *timer, TickTimer_TimerStateType *state)
{
   
    

    if (true == TickTimer_initialized)
    {
        if (((timer->startTs ^ timer->inverseStartTs) != 0xffffu) || ((timer->value ^ timer->inverseValue) != 0xffffu))
        {
            *state = TickTimer_Uninitialized;
            
        }
        
        if ((uint16_t)(counter - timer->startTs) >= timer->value)
        {
            *state = TickTimer_Expired;
        }
        else
        {
            *state = TickTimer_Running;
        }
       
    }

    
}

void TickTimer_Tick(void)
{
  

    if (true == TickTimer_initialized)
    {
        counter += 1u;
      
    }

   
}