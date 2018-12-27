/*************************************************************************
 * realtime.cpp
 *
 * Created: 20.12.2018 11:58:20
 * Revised:
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include "realtime.h"

 REALTIME::REALTIME()
{
    CLK.RTCCTRL = (0x06<<1)|CLK_RTCEN_bm;
    RTC.INTCTRL = RTC_OVFINTLVL_HI_gc;
    RTC.CNT = 0;
    while (RTC.STATUS & RTC_SYNCBUSY_bm);
    RTC.PER = 0x7FFF;
    while (RTC.STATUS & RTC_SYNCBUSY_bm);
    RTC.CTRL = RTC_PRESCALER_DIV1_gc;
    while (RTC.STATUS & RTC_SYNCBUSY_bm);
}

void REALTIME::SetTime(uint8_t unTimeHours, uint8_t unTimeMinutes, uint8_t unTimeSeconds)
{
    m_unTimeHours = unTimeHours;
    m_unTimeMinutes = unTimeMinutes;
    m_unTimeSeconds = unTimeSeconds;
}

void REALTIME::StartCounter(void pvTask(), uint16_t unTimeoutS)
{
    m_nMatch = (m_nCounter + unTimeoutS) & 0x7FFFFFFF;
    m_pvTask = (void*)pvTask;
}

void REALTIME::InterruptHandler()
{
    /* Timeout counter */
    m_nCounter++;
    if (m_nCounter < 0) {m_nCounter = 0;}
    if (m_nCounter >= m_nMatch && m_pvTask)
    {
        cMTask.Run((FuncPtr_t)m_pvTask);
        m_nMatch = -1;
    }
    
    /* Real time */
    m_unTimeSeconds++;
    if (m_unTimeSeconds == 60)
    {
        m_unTimeSeconds = 0;
        m_unTimeMinutes++;
    }
    if (m_unTimeMinutes == 60)
    {
        m_unTimeMinutes = 0;
        m_unTimeHours++;
    }
    if (m_unTimeHours == 24)
    {
        m_unTimeHours = 0;
    }
}