/*************************************************************************
 * conf_data.h
 *
 * Created: 24.6.2018 17:09:11
 * Revised: 15.7.2018
 * Author: LeXa
 * BOARD:
 *
 * ABOUT:
 *
 *************************************************************************/

#ifndef CONF_DATA_H_
#define CONF_DATA_H_

/************************************************************************/
/* ENUMs                                                                */
/************************************************************************/
enum DEVICE_TYPE_enum {
    DEVICE_TYPE_BOOT_LOADER,
    DEVICE_TYPE_MASTER_CONTROLLER,
    DEVICE_TYPE_LED_CONTROLLER,
    DEVICE_TYPE_METEO_STATION,
};

enum ERROR_enum {
    ERROR_OK,
    ERROR_OUT_OF_RANGE,
    ERROR_OVERVOLTAGE,
    ERROR_UNDERVOLTAGE,
    ERROR_UNKNOWN_COMMAND,
    ERROR_NEXT,
    ERROR_VERIFICATION_FAILED,
};

enum LEDChConfig_enum {
    LEDCH_Disabled,
    LEDCH_PWM,
    LEDCH_OnOff,
};

/************************************************************************/
/* DATASET                                                              */
/************************************************************************/
union DATASET_RF {
    uint8_t reg[16];
    struct {
        uint8_t                 unRFChannel;                /* RF channel number                        */
        uint8_t                 aTransmitAddress[5];        /* Slave address                            */
        uint8_t                 unRFBaud;                   /* RF Baud rate (250kbps, 1Mbps, 2Mbps)     */
        uint32_t                unAppChecksum;              /* Application sector checksum              */
        uint8_t                 aReserved[5];               /* Reserved bytes for future use            */
    };
};

union DATASET_LED {
    uint8_t reg[10];
    struct {
        LEDChConfig_enum        ePwmCH1Config;              /* PWM channel configuration                                    */
        LEDChConfig_enum        ePwmCH2Config;              /* PWM channel configuration                                    */
        uint8_t                 unPwmCH1MaxVal;             /* Maximal value of channel. 1bit = 1%                          */
        uint8_t                 unPwmCH2MaxVal;             /* Maximal value of channel. 1bit = 1%                          */
        uint8_t                 bBothEdges:1;               /* 1 - switch on both edges,    0 - switch on failing edge      */
        uint8_t                 bChSeparationEnable:1;      /* 1 - channels separated,      0 - channels switched together  */
        uint8_t                 bCh1First:1;                /* 1 - channel 1 first,         0 - channel 2 first             */
        int8_t                  nVoltageOffset;             /* Voltage offset 1bit = 0.1V, resolution -12.7V..+12.8V        */
        uint16_t                unTimeoutMin;               /* Light off timeout 1bit - 1 minute                            */
        uint8_t                 unIncSpeed;                 /* PWM increase speed. 1-fastest .. 100-slowest                 */
        uint8_t                 unDecSpeed;                 /* PWM decrease speed. 1-fastest .. 100-slowest                 */
    };
};    

/************************************************************************/
/* COMMANDS                                                             */
/************************************************************************/
enum RF_COMMAND_enum {
    /* Basic commands */
    RF_COMM_RESET,
    RF_COMM_STATUS,
    RF_COMM_LED_GET_DATASET,
    RF_COMM_LED_SET_DATASET,
    RF_COMM_LED_SET_PWM,
    RF_COMM_LED_CLEAR_LIGHTON_COUNTER,
    
    /* Error answer to command. Error code is followed */
    RF_COMM_ERROR = 255,
};

/************************************************************************/
/* RF COMUNICATION STRUCTURE                                            */
/************************************************************************/
union RF_DATA {
    uint8_t reg[32];
    struct 
    {
        /*===== COMMAND =========================================================
         * Command is always in first place
         *=======================================================================*/
        RF_COMMAND_enum eRfCommand;
        
        union 
        {
            /*===== MASTER DATA =====================================================
             * Data send by Master controller
             *=======================================================================*/
            union 
            {
                DATASET_LED                 SetDataset;
                struct SET_PWM_struct {
                    uint8_t unPwmChannel;                   /* PWM channel */
                    uint8_t unPercent;                      /* 1bit = 1% */
                } sSetPwm;
            } Master;
            
            /*===== SLAVE DATA ======================================================
             * Data send by Slave controller
             *=======================================================================*/
            union 
            {
                ERROR_enum                  eError;
                struct STATUS_LED_struct {
                    DEVICE_TYPE_enum eDeviceType;
                    uint8_t unSupplyVolt;                   /* 1bit = 0,1V */
                    uint8_t unPwmCH1Percent;                /* 1bit = 1% */
                    uint8_t unPwmCH2Percent;                /* 1bit = 1% */
                    uint16_t unTimeLightOn;                 /* 1bit = 1s */
                } sStatusLed;
                DATASET_LED                 GetDataset;
            } Slave;
        };
    };
};

#endif /* CONF_DATA_H_ */