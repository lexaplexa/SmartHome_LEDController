/*************************************************************************
 * task_light_switch.cpp
 *
 * Created: 11.2.2016 13:40:59
 * Revised: 24.6.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 * 
 *************************************************************************/

#include <app/app.h>

PWM cPwmCH1(&PWM_PORT,&PWM_TIMER,PWMCH1_PIN,PORT_OPC_TOTEM_gc,F_CPU,PWM_FRQ_HZ);
PWM cPwmCH2(&PWM_PORT,&PWM_TIMER,PWMCH2_PIN,PORT_OPC_TOTEM_gc,F_CPU,PWM_FRQ_HZ);
int16_t nCH1SetPwmVal;
int16_t nCH2SetPwmVal;

void SetCH1(uint8_t unPercent, uint16_t unDelay)
{
	nCH1SetPwmVal = unPercent*10;
	if (nCH1SetPwmVal > DSLed.unPwmCH1MaxVal) {nCH1SetPwmVal = DSLed.unPwmCH1MaxVal*10;}
	if		(DSLed.ePwmCH1Config == LEDCH_Disabled)						{nCH1SetPwmVal = 0; cPwmCH1.SetWidth(0); return;}
	else if	(DSLed.ePwmCH1Config == LEDCH_OnOff && nCH1SetPwmVal > 0)	{cPwmCH1.SetWidth(100);}
	else if (DSLed.ePwmCH1Config == LEDCH_OnOff && nCH1SetPwmVal == 0)	{cPwmCH1.SetWidth(0);}
	else if (DSLed.ePwmCH1Config == LEDCH_PWM)							{cMTask.Delay(taskCH1PWM,unDelay);}
	else {return;}
    
    /* Keep at least one task active to prevent against deep sleep */
    cMTask.Delay(taskSleep,TASK_TOUT_MS(100));
}

void taskCH1PWM()
{
	if		(cPwmCH1.m_nPercent > nCH1SetPwmVal) {cPwmCH1.Add(-1); cMTask.Delay(taskCH1PWM,TASK_TOUT_MS(PWM_DELAY_DEC_MS));}
	else if (cPwmCH1.m_nPercent < nCH1SetPwmVal) {cPwmCH1.Add(+1); cMTask.Delay(taskCH1PWM,TASK_TOUT_MS(PWM_DELAY_INC_MS));} 
}

void SetCH2(uint8_t unPercent, uint16_t unDelay)
{
	nCH2SetPwmVal = unPercent*10;
	if (nCH2SetPwmVal > DSLed.unPwmCH2MaxVal) {nCH2SetPwmVal = DSLed.unPwmCH2MaxVal*10;}
	if		(DSLed.ePwmCH2Config == LEDCH_Disabled)						{nCH2SetPwmVal = 0; cPwmCH2.SetWidth(0); return;}
	else if	(DSLed.ePwmCH2Config == LEDCH_OnOff && nCH2SetPwmVal > 0)	{cPwmCH2.SetWidth(100);}
	else if (DSLed.ePwmCH2Config == LEDCH_OnOff && nCH2SetPwmVal == 0)	{cPwmCH2.SetWidth(0);}
	else if (DSLed.ePwmCH2Config == LEDCH_PWM)							{cMTask.Delay(taskCH2PWM,unDelay);}
	else {return;}
    
    /* Keep at least one task active to prevent against deep sleep */
    cMTask.Delay(taskSleep,TASK_TOUT_MS(100));
}

void taskCH2PWM()
{
	if		(cPwmCH2.m_nPercent > nCH2SetPwmVal) {cPwmCH2.Add(-1); cMTask.Delay(taskCH2PWM,TASK_TOUT_MS(PWM_DELAY_DEC_MS));}
	else if (cPwmCH2.m_nPercent < nCH2SetPwmVal) {cPwmCH2.Add(+1); cMTask.Delay(taskCH2PWM,TASK_TOUT_MS(PWM_DELAY_INC_MS));}
}

void taskLongPressSwitch()
{
	if (~SWITCH_PORT.IN & (1<<SWITCH_PIN))
	{
		if (!cPwmCH1.m_nPercent)		{SetCH1(100,0);}
		else if (!cPwmCH2.m_nPercent)	{SetCH2(100,0);}
		else							{SetCH1(0,0); SetCH2(0,0);}
	}
}


SIGNAL(SWITCH_INT_vect)
{
	/* Wake up RF and set time event to -1 to be sure, that RF doesn't sleep again */
	cRf.WakeUp();
		
	if (DSLed.bBothEdges)
	{
		if (SWITCH_PORT.IN & (1<<SWITCH_PIN)) {SetCH1(0,0); SetCH2(0,0);}
		else {SetCH1(100,0); SetCH2(100,0);}
	}
	else if (!DSLed.bChSeparationEnable)
	{
		if (!nCH1SetPwmVal && !nCH2SetPwmVal)	{SetCH1(100,0); SetCH2(100,0);}
		else									{SetCH1(0,0); SetCH2(0,0);}
	}	
	else
	{
		if (DSLed.bCh1First) 
		{
			if (!cPwmCH1.m_nPercent)		{SetCH1(100,0); cMTask.Delay(taskLongPressSwitch,TASK_TOUT_MS(400));}
			else if (!cPwmCH2.m_nPercent)	{SetCH2(100,TASK_TOUT_MS(400)); cMTask.Delay(taskLongPressSwitch,TASK_TOUT_MS(400));}
			else							{SetCH1(0,0); SetCH2(0,0);}
		}		
		else
		{
			if (!cPwmCH2.m_nPercent)		{SetCH2(100,0); cMTask.Delay(taskLongPressSwitch,TASK_TOUT_MS(400));}
			else if (!cPwmCH1.m_nPercent)	{SetCH1(100,TASK_TOUT_MS(400)); cMTask.Delay(taskLongPressSwitch,TASK_TOUT_MS(400));}
			else							{SetCH1(0,0); SetCH2(0,0);}
		}		
	}
}