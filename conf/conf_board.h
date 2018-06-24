/*************************************************************************
 * conf_board.h
 *
 * Created: 11.2.2016 13:01:57
 * Revised: 24.6.2018
 * Author: LeXa
 * BOARD: LED_LIGHT_E2
 *
 * ABOUT:
 * Configuration of board
 *************************************************************************/

#ifndef CONF_BOARD_H_
#define CONF_BOARD_H_

/************************************************************************/
/* BOARD                                                                */
/************************************************************************/
#define LED_LIGHT_E					/* Board name and version */


/************************************************************************/
/* SYSTEM CLOCK OPTIONS                                                 */
/************************************************************************/

	#define F_CPU						CONF_SOURCE_FREQ*CONF_FREQ_MULTIPLICATION
	#define CONF_SOURCE_FREQ			32000000
	#define CONF_FREQ_MULTIPLICATION	1
	
	/* Define source for PLL if used. Uncomment the right one */
	//#define CONF_PLL_SOURCE			OSC_PLLSRC_XOSC_gc
	//#define CONF_PLL_SOURCE			OSC_PLLSRC_RC2M_gc
	//#define CONF_PLL_SOURCE			OSC_PLLSRC_RC32M_gc


/************************************************************************/
/* HARDWARE OPTIONS                                                     */
/************************************************************************/

    /*===== RF MODULE nRF24L01+ ============================================
	 *======================================================================*/
	#define RF_PORT							PORTC
	#define RF_SPI							SPIC
	#define RF_BAUD_SPI						1000000
	#define RF_IRQ_vect						PORTC_INT0_vect
	#define RF_IRQ_PIN						PIN2_bp
	#define RF_CE_PIN						PIN3_bp
	#define ENABLE_AUTOACK					false
	#define DYNAMIC_PAYLOAD					true
	#define STATIC_PAYLOAD_BUFFER_LENGTH	32
	#define DATA_PIPE0_EN					1
    
	/*===== SWITCH INPUT ===================================================
	 *======================================================================*/
	#if defined LED_LIGHT_B || defined LED_LIGHT_C || defined LED_LIGHT_E
		#define SWITCH_PORT				PORTB
		#define SWITCH_PIN				PIN2_bp
		#define SWITCH_INT_vect			PORTB_INT0_vect
	#elif defined LED_LIGHT_D
		//#define SWITCH_PORT				PORTD
		//#define SWITCH_PIN				PIN2_bp	
		//#define SWITCH_INT_vect			PORTD_INT0_vect
	#endif
	
	/*===== ADC ============================================================
	 *======================================================================*/
	#define SUPPLY_MUX_PIN				ADC_CH_MUXPOS_PIN1_gc
	#define R101						24000
	#define R102						1000
	#define SUPPLY_RATIO				(R101+R102)/R102
	
	/*===== IR RECEIVER ====================================================
	 *======================================================================*/
	#ifdef LED_LIGHT_B||LED_LIGHT_C
		#define IR_ENABLED
	#endif	
	#define IR_COUNTER					TCC1
	#define IR_COUNTER_OVF_vect			TCC1_OVF_vect
	#define IR_PORT						PORTA
	#define IR_PORT_INT_vect			PORTA_INT0_vect	
	#define IR_PIN						PIN2_bp

	/*===== PWM ============================================================
	 * Setting PWM for LED illumination
	 *======================================================================*/
	#define PWM_PORT					PORTE
	#define PWM_TIMER					TCE0
	#define PWMCH1_PIN					PIN0_bp
	#define PWMCH2_PIN					PIN1_bp
	#define PWM_FRQ_HZ					270
	#define PWM_DELAY_INC_MS			10
	#define PWM_DELAY_DEC_MS			10

#endif /* CONF_BOARD_H_ */