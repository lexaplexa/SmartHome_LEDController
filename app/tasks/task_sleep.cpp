/*************************************************************************
 * task_sleep.cpp
 *
 * Created: 23.3.2017 15:31:46
 * Revised: 24.6.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 * 
 *************************************************************************/

#include <app/app.h>

void taskSleep()
{
	/* If lights on, then this task must be active to prevent controller from deep sleep */
	if (cPwmCH1.m_nPercent || cPwmCH2.m_nPercent) {cMTask.Delay(taskSleep,TASK_TOUT_MS(100));}
}