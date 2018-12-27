/*************************************************************************
 * task_startupapp.cpp
 *
 * Created: 24.6.2018 10:37:08
 * Revised: 27.12.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#include <app/app.h>

DATASET_LED DSLed;
DATASET_LED EEDSLed EEMEM = {
    LEDCH_PWM,
    LEDCH_PWM,
    100,
    100,
    0,
    0,
    0,0,
    10,
    10,
};
DATASET_RF DSRf;
DATASET_RF EEDSRf EEMEM = {
    /* Default EEPROM Dataset Values ---------------------------------------*/
    125,                        /* RF channel number                        */
    0xAA,0x55,0xAA,0x55,0xAA,   /* Slave address                            */
    RF_BAUD_250kbps_gc,         /* RF Baud rate (250kbps, 1Mbps, 2Mbps)     */
    0x00000000                  /* CRC application sector checksum          */
};


void taskStartUpApp()
{
    /* Load Datasets from EEPROM */
    eeprom_read_block(DSLed.reg, EEDSLed.reg, sizeof(DATASET_LED));
    eeprom_read_block(DSRf.reg, EEDSRf.reg, sizeof(DATASET_RF));
    
    /* Switch button */
    SWITCH_PORT.INT0MASK = 1<<SWITCH_PIN;
    if (DSLed.bBothEdges) {SWITCH_PORT.PIN2CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_BOTHEDGES_gc;}
    else {SWITCH_PORT.PIN2CTRL = PORT_OPC_PULLUP_gc|PORT_ISC_FALLING_gc;}
    SWITCH_PORT.INTCTRL = PORT_INT0LVL_LO_gc;
    
    /* PWM channels */
    cPwmCH1.SetWidth(0);
    cPwmCH2.SetWidth(0);
    cPwmCH1.Run();
    cPwmCH2.Run();
    
    /* ADC initialization */
    adc_init(ADCA,ADC_PRESCALER_DIV256_gc,ADC_RESOLUTION_12BIT_gc);
    
    /* RF initialization */
    cRf.Init();
    cRf.SetBaud((RF_BAUD_enum)DSRf.unRFBaud);
    cRf.SetChannel(DSRf.unRFChannel);
    cRf.SetTXaddress(DSRf.aTransmitAddress,5);
    cRf.SetRXaddress(DSRf.aTransmitAddress,5,0);
    cRf.WakeUp();
    
    /* Send message after start. Slave identifies itself */
    RFData.eRfCommand = RF_COMM_STATUS;
    cRf.m_eStatus = RF_STATUS_RECEIVE_OK;
    taskRf();
    
    cMTask.DeepSleepEnable();
}