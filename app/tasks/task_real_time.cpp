/*************************************************************************
 * task_real_time.cpp
 *
 * Created: 27.3.2017 13:32:05
 * Revised: 27.12.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

REALTIME cRtc;
uint16_t unTimeLightsOn;

void taskLightOff()
{
    SetCH1(0,0);
    SetCH2(0,0);
}

SIGNAL(RTC_OVF_vect)
{
    cRtc.InterruptHandler();
    if (nCH1SetPwmVal || nCH2SetPwmVal)
    {
        /* Increment timeout and light on counter */
        unTimeLightsOn++;
    }
}    