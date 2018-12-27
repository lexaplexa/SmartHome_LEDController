/*************************************************************************
 * app.h
 *
 * Created: 24.6.2018 10:34:46
 * Revised: 27.12.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/


#ifndef APP_H_
#define APP_H_

#include <core/core.h>

/************************************************************************/
/* EXTERNAL CLASSES                                                     */
/************************************************************************/
extern DATASET_LED DSLed;           /* app\tasks\task_startupapp.cpp */
extern DATASET_LED EEDSLed;         /* app\tasks\task_startupapp.cpp */
extern DATASET_RF DSRf;             /* app\tasks\task_startupapp.cpp */
extern DATASET_RF EEDSRf;           /* app\tasks\task_startupapp.cpp */
extern class RF cRf;                /* app\tasks\task_rf.cpp */
extern RF_DATA RFData;              /* app\tasks\task_rf.cpp */
extern class PWM cPwmCH1;           /* app\tasks\task_light_switch.cpp */
extern class PWM cPwmCH2;           /* app\tasks\task_light_switch.cpp */
extern int8_t nCH1SetPwmVal;        /* app\tasks\task_light_switch.cpp */
extern int8_t nCH2SetPwmVal;        /* app\tasks\task_light_switch.cpp */
extern class REALTIME cRtc;         /* app\tasks\task_real_time.cpp */
extern uint16_t unTimeLightsOn;     /* app\tasks\task_real_time.cpp */

/************************************************************************/
/* TASK PROTOTYPES                                                      */
/*----------------------------------------------------------------------*/
/* Every task function is named void task<name of task>();              */
/* For example:                                                         */
/* void taskStartUp();                                                  */
/*                                                                      */
/* File names with source code are named task_<name of task>.cpp        */
/* For example:                                                         */
/* task_startup.cpp                                                     */
/*                                                                      */
/* Every task source code must include core.h                           */
/************************************************************************/
void taskRf();                      /* app\tasks\task_rf.cpp */
void taskSleep();                   /* app\tasks\task_sleep.cpp */
void taskCH1PWM();                  /* app\tasks\task_light_switch.cpp */
void taskCH2PWM();                  /* app\tasks\task_light_switch.cpp */
void taskLightOff();                /* app\tasks\task_real_time.cpp */

void SetCH1(uint8_t unPercent, uint16_t unDelay);   /* app\tasks\task_light_switch.cpp */
void SetCH2(uint8_t unPercent, uint16_t unDelay);   /* app\tasks\task_light_switch.cpp */

#endif /* APP_H_ */