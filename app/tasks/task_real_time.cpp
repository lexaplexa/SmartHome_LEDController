/*************************************************************************
 * task_real_time.cpp
 *
 * Created: 27.3.2017 13:32:05
 * Revised: 18.8.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

uint16_t unTimeoutSeconds;
uint16_t unTimeLightsOn;

SIGNAL(RTC_OVF_vect)
{    
    if (nCH1SetPwmVal || nCH2SetPwmVal)
    {
        /* Increment timeout and light on counter */
        unTimeLightsOn++;
        unTimeoutSeconds++;
        if (DSLed.unTimeoutMin != 0 && unTimeoutSeconds >= (DSLed.unTimeoutMin*60))
        {
            SetCH1(0,0);
            SetCH2(0,0);
            unTimeoutSeconds = 0;
        }
    }
    else
    {
        unTimeoutSeconds = 0;
    }
}