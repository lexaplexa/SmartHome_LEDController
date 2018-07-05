/*************************************************************************
 * task_rf.cpp
 *
 * Created: 15.2.2016 14:25:22
 * Revised: 5.7.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

RF cRf;
RF_DATA RFData;

void taskRf()
{
    uint8_t unRfMsgSize;
    
    if (cRf.m_eStatus == RF_STATUS_RECEIVE_OK) {memcpy(RFData.reg, cRf.m_sPayload.punBuffer, cRf.m_sPayload.unCounter);}
    else {return;}
    
    /* Send answer to Master */
    switch (RFData.eRfCommand)
    {
        case RF_COMM_RESET:
            CCP = CCP_IOREG_gc; RST.CTRL = RST_SWRST_bm;
            break;
        
        
        case RF_COMM_STATUS:
            int16_t nAdcRes;
            float fVolt;
            
            RFData.Slave.sStatusLed.eDeviceType = DEVICE_TYPE_LED_CONTROLLER;
            /* Read supply voltage */
            adc_read(ADCA,SUPPLY_MUX_PIN,nAdcRes);
            fVolt = ADC_TO_VOLT((float)nAdcRes*SUPPLY_RATIO*10);
            RFData.Slave.sStatusLed.unSupplyVolt = (uint8_t)fVolt + DSLed.nVoltageOffset;
            
            /* PWM channel value */
            RFData.Slave.sStatusLed.unPwmCH1Percent = nCH1SetPwmVal;
            RFData.Slave.sStatusLed.unPwmCH2Percent = nCH2SetPwmVal;
            RFData.Slave.sStatusLed.unTimeLightOn = unTimeLightsOn;
            unRfMsgSize = 1+sizeof(RFData.Slave.sStatusLed);
            break;
        
            
        case RF_COMM_LED_GET_DATASET:
            memcpy(RFData.Slave.GetDataset.reg, DSLed.reg, sizeof(DATASET_LED));
            unRfMsgSize = 1+sizeof(DATASET_LED);
            break;
            
            
        case RF_COMM_LED_SET_DATASET:
            if (cRf.m_sPayload.unCounter == (sizeof(DATASET_LED)+1))
            {
                /* Copy data to Dataset */
                memcpy(DSLed.reg, RFData.Slave.GetDataset.reg, sizeof(DATASET_LED));
                /* Copy Dataset to EEPROM */
                eeprom_write_block(&RFData.Slave.GetDataset.reg, &EEDSLed.reg, sizeof(DATASET_LED));
                if (DSLed.bBothEdges)   {SWITCH_PORT.PIN2CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_BOTHEDGES_gc;}
                else                    {SWITCH_PORT.PIN2CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_FALLING_gc;}
                RFData.Slave.eError = ERROR_OK;
            }
            else {RFData.eRfCommand = RF_COMM_ERROR; RFData.Slave.eError = ERROR_OUT_OF_RANGE;}
            unRfMsgSize = 2;
            break;
            
            
        case RF_COMM_LED_SET_PWM:
            if (RFData.Master.sSetPwm.unPwmChannel == 1)
            {
                if (cRf.m_sPayload.unCounter == 3)  {SetCH1(RFData.Master.sSetPwm.unPercent,0);}
                else if (nCH1SetPwmVal)             {SetCH1(0,0);}
                else                                {SetCH1(100,0);}
                
                if (!nCH2SetPwmVal) {DSLed.bCh1First = true;}
                else {DSLed.bCh1First = false;}
                RFData.Slave.eError = ERROR_OK; 
            }
            else if (RFData.Master.sSetPwm.unPwmChannel == 2)
            {
                if (cRf.m_sPayload.unCounter == 3)  {SetCH2(RFData.Master.sSetPwm.unPercent,0);}
                else if (nCH2SetPwmVal)             {SetCH2(0,0);}
                else                                {SetCH2(100,0);}
                    
                if (!nCH1SetPwmVal) {DSLed.bCh1First = false;}
                else {DSLed.bCh1First = true;}
                RFData.Slave.eError = ERROR_OK;
            }
            else if (RFData.Master.sSetPwm.unPwmChannel == 3)
            {
                if (cRf.m_sPayload.unCounter == 3)          {SetCH1(RFData.Master.sSetPwm.unPercent,0); SetCH2(RFData.Master.sSetPwm.unPercent,0);}
                else if (nCH2SetPwmVal || nCH1SetPwmVal)    {SetCH1(0,0); SetCH2(0,0);}
                else                                        {SetCH1(100,0); SetCH2(100,0);}
                RFData.Slave.eError = ERROR_OK;
            }
            else {RFData.eRfCommand = RF_COMM_ERROR; RFData.Slave.eError = ERROR_OUT_OF_RANGE;}
            unRfMsgSize = 2;
            break;
            
            
        case RF_COMM_LED_CLEAR_LIGHTON_COUNTER:
            unTimeLightsOn = 0;
            RFData.Slave.eError = ERROR_OK;
            unRfMsgSize = 2;
            break;
        
        
        default:
            RFData.eRfCommand = RF_COMM_ERROR;
            RFData.Slave.eError = ERROR_UNKNOWN_COMMAND;
            unRfMsgSize = 2;
            break;
    }
    cRf.SendMsg(RFData.reg,unRfMsgSize);
}

SIGNAL(RF_IRQ_vect)
{
    cRf.InterruptHandler();
    if (DSRf.unRFBaud == (RF_BAUD_enum)RF_BAUD_250kbps_gc)      {cMTask.Delay(taskRf,TASK_TOUT_MS(10));}
    else if (DSRf.unRFBaud == (RF_BAUD_enum)RF_BAUD_1Mbps_gc)   {cMTask.Delay(taskRf,TASK_TOUT_MS(5));}
    else                                                        {cMTask.Delay(taskRf,TASK_TOUT_MS(1));}
    cMTask.Delay(taskSleep,TASK_TOUT_MS(100));
}