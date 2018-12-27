/*************************************************************************
 * realtime.h
 *
 * Created: 20.12.2018 10:37:21
 * Revised:
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#ifndef REALTIME_H_
#define REALTIME_H_

#include <avr/io.h>
#include <core/core.h>

/************************************************************************/
/* CLASS                                                                */
/************************************************************************/
class REALTIME
{
    private:
        int32_t m_nCounter;
        int32_t m_nMatch = -1;
        void* m_pvTask;
        uint8_t m_unTimeSeconds;
        uint8_t m_unTimeMinutes;
        uint8_t m_unTimeHours;
        
    public:
        REALTIME();
        void SetTime(uint8_t unTimeHours, uint8_t unTimeMinutes, uint8_t unTimeSeconds);
        void StartCounter(void pvTask(), uint16_t unTimeoutS);
        void InterruptHandler();
};


#endif /* REALTIME_H_ */