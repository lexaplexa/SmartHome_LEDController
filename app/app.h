/*************************************************************************
 * app.h
 *
 * Created: 24.6.2018 10:34:46
 * Revised:
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
extern int16_t nCH1SetPwmVal;       /* app\tasks\task_light_switch.cpp */
extern int16_t nCH2SetPwmVal;       /* app\tasks\task_light_switch.cpp */
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
void taskRf();
void taskSleep();
void taskCH1PWM();
void taskCH2PWM();

void SetCH1(uint8_t unPercent, uint16_t unDelay);
void SetCH2(uint8_t unPercent, uint16_t unDelay);

#endif /* APP_H_ */