/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

#include "gd32f30x.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f303e_eval.h"

#define PWM_FREQ ((uint16_t) 16000) // in Hz  (N.b.: pattern type is center aligned)
#define PWM_PRSC ((uint8_t)0)
#define HALF_PWM_PERIOD ((uint16_t) (CKTIM / (uint32_t)(2 * PWM_FREQ *(PWM_PRSC+1)))) 

#define CKTIM	((uint32_t)72000000uL) 	/* Silicon running at 72MHz Resolution: 1Hz */
#define DEADTIME_NS	((uint16_t) 500)  //in nsec; range is [0...3500]
#define DEADTIME  (uint16_t)((unsigned long long)CKTIM / 2 * (unsigned long long)DEADTIME_NS/1000000000uL)
#define ADC_PRE_PRIORITY (uint8_t)1
#define ADC_SUB_PRIORITY (uint8_t)1

#define RCU_TIMERxx RCU_TIMER0
#define TIMERxx TIMER0
#define rate 1199

/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal){

        if(timingdelaylocal < 500U){
            gd_eval_led_on(LED1);
        }else{
            gd_eval_led_off(LED1);
        }

        timingdelaylocal--;
    }else{
        timingdelaylocal = 1000U;
    }
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/

int main(void)
{
	  uint32_t dummy = 0;
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocintpara;
    timer_break_parameter_struct timer_bkdtpara;
	
	  uint32_t i;
	
    /* configure systick */
    systick_config();
    /* initilize the LEDs, USART and key */
    gd_eval_led_init(LED1);
    //gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);  //PA8

    /************************************* step0: 开启时钟 *******************************************/
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_GPIOE);
		rcu_periph_clock_enable(RCU_AF);
    rcu_periph_clock_enable(RCU_TIMERxx);
    rcu_periph_clock_enable(RCU_ADC0);
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV2);
		
		//AFIO_PCF0|=1<<2;  //UART0 PB6 PB7
    //gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);  //UART0 PB6 PB7
    //gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    //gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    gd_eval_com_init(EVAL_COM1);  //PA9 PA10
		
		/* print out the clock frequency of system, AHB, APB1 and APB2 */
    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));
    
    /************************************* step1: GPIO配置 *******************************************/
    /* 配置6路PWM的IO */
		//gpio_pin_remap_config(GPIO_TIMER0_PARTIAL_REMAP, ENABLE);
    //gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    //gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_11);
    //gpio_init(GPIOE, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_13);
		gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
		
		gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
		gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
		gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_0);

    /* 驱动使能 */
    gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_bit_reset(GPIOA, GPIO_PIN_5);
    
    /* 配置电流采样的IO */
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_1);
    gpio_init(GPIOA, GPIO_MODE_AIN, GPIO_OSPEED_50MHZ, GPIO_PIN_2);


    /************************************* step2: timer配置 ******************************************/
    timer_deinit(TIMERxx);
    timer_initpara.prescaler = 2;//PWM_PRSC;
    timer_initpara.alignedmode = TIMER_COUNTER_EDGE;
		timer_initpara.counterdirection = TIMER_COUNTER_UP;
    timer_initpara.period = 1199;//HALF_PWM_PERIOD;          /* 配置三相桥开关频率 */
    timer_initpara.clockdivision = TIMER_CKDIV_DIV1;  /* 死区时间计算用到的分频 */
    timer_initpara.repetitioncounter = 1;                /* 重复计数器, 影响更新事件的频率 */
    timer_init(TIMERxx, &timer_initpara);
    
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_DISABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;     
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    
    timer_channel_output_config(TIMERxx, TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMERxx, TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMERxx, TIMER_CH_2,&timer_ocintpara);
    
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMERxx, TIMER_CH_3,&timer_ocintpara);
    
    /* channel1~3 */
    timer_channel_output_pulse_value_config(TIMERxx, TIMER_CH_0, 30/*HALF_PWM_PERIOD/2*/);
    timer_channel_output_mode_config(TIMERxx, TIMER_CH_0, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMERxx, TIMER_CH_0, TIMER_OC_SHADOW_DISABLE);   /* 预装载使能 */
    timer_channel_output_pulse_value_config(TIMERxx, TIMER_CH_1, 30/*HALF_PWM_PERIOD/2*/);
    timer_channel_output_mode_config(TIMERxx, TIMER_CH_1, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMERxx, TIMER_CH_1, TIMER_OC_SHADOW_DISABLE);   /* 预装载使能 */
    timer_channel_output_pulse_value_config(TIMERxx, TIMER_CH_2, 30/*HALF_PWM_PERIOD/2*/);
    timer_channel_output_mode_config(TIMERxx, TIMER_CH_2, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMERxx, TIMER_CH_2, TIMER_OC_SHADOW_DISABLE);   /* 预装载使能 */
    
    /* channel4 设置, 触发ADC用 */
    timer_channel_output_pulse_value_config(TIMERxx, TIMER_CH_3, 5);
    timer_channel_output_mode_config(TIMERxx, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMERxx, TIMER_CH_3, TIMER_OC_SHADOW_ENABLE);   // 预装载使能
    timer_master_output_trigger_source_select(TIMERxx, TIMER_TRI_OUT_SRC_O3CPRE);      // 上升沿触发ADC
    
    /* 配置死区 */
    timer_bkdtpara.runoffstate = TIMER_ROS_STATE_ENABLE;
    timer_bkdtpara.ideloffstate = TIMER_IOS_STATE_ENABLE;
    timer_bkdtpara.protectmode = TIMER_CCHP_PROT_OFF;
    timer_bkdtpara.deadtime = 21;//DEADTIME;
    timer_bkdtpara.breakstate = TIMER_BREAK_DISABLE;
    timer_bkdtpara.breakpolarity = TIMER_BREAK_POLARITY_LOW;
    timer_bkdtpara.outputautostate = TIMER_OUTAUTO_ENABLE;
    timer_break_config(TIMERxx, &timer_bkdtpara);

    /* 多电机控制同步用的 */
    timer_input_trigger_source_select(TIMERxx, TIMER_SMCFG_TRGSEL_ITI1);
    timer_slave_mode_select(TIMERxx, TIMER_SLAVE_MODE_EVENT);
    timer_event_software_generate(TIMERxx, TIMER_EVENT_SRC_UPG);
    timer_counter_value_config(TIMERxx, 0);
    
    /* 只需要CC4使能 */
    timer_channel_output_state_config(TIMERxx, TIMER_CH_0, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMERxx, TIMER_CH_1, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMERxx, TIMER_CH_2, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMERxx, TIMER_CH_3, TIMER_CCX_ENABLE);
    timer_channel_complementary_output_state_config(TIMERxx, TIMER_CH_0, TIMER_CCXN_DISABLE);
    timer_channel_complementary_output_state_config(TIMERxx, TIMER_CH_1, TIMER_CCXN_DISABLE);
    timer_channel_complementary_output_state_config(TIMERxx, TIMER_CH_2, TIMER_CCXN_DISABLE);
    
    //timer_update_source_config(TIMERxx, TIMER_UPDATE_SRC_REGULAR);
    //timer_single_pulse_mode_config(TIMERxx, TIMER_SP_MODE_REPETITIVE);
    timer_primary_output_config(TIMERxx, ENABLE);
    timer_auto_reload_shadow_enable(TIMERxx);
    timer_enable(TIMERxx);

    //dbg_periph_enable(DBG_TIMER0_HOLD);

    /************************************** step3: adc配置 *******************************************/
    adc_deinit(ADC0);
    adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, DISABLE);
    adc_external_trigger_source_config(ADC0, ADC_REGULAR_CHANNEL, ADC0_1_2_EXTTRIG_REGULAR_NONE); 
    adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
    
    /* 注入组配置 */
    /*adc_channel_length_config(ADC0, ADC_INSERTED_CHANNEL, 2);
    adc_inserted_channel_config(ADC0, 0, ADC_CHANNEL_1, ADC_SAMPLETIME_7POINT5);         // A相电流
    adc_inserted_channel_config(ADC0, 1, ADC_CHANNEL_2, ADC_SAMPLETIME_7POINT5);         // B相电流
    
    adc_external_trigger_source_config(ADC0, ADC_INSERTED_CHANNEL, ADC0_1_EXTTRIG_INSERTED_T0_CH3); 
    adc_external_trigger_config(ADC0, ADC_INSERTED_CHANNEL, ENABLE);

    adc_enable(ADC0);
    for(dummy = 0; dummy < 60000; dummy++);
    adc_calibration_enable(ADC0);
    
    adc_interrupt_enable(ADC0, ADC_INT_EOIC);
    nvic_irq_enable(ADC0_1_IRQn, ADC_PRE_PRIORITY, ADC_SUB_PRIORITY);
    */
		
    TIMER_CHCTL2(TIMERxx)|=0x0111;
		TIMER_CH0CV(TIMERxx)=500;
		TIMER_CH1CV(TIMERxx)=500;
		TIMER_CH2CV(TIMERxx)=500;
    while(1) {
			for (i=0;i<rate+1;i=i+8){
				TIMER_CH0CV(TIMERxx)=i;
				TIMER_CH1CV(TIMERxx)=rate-i;
				TIMER_CH2CV(TIMERxx)=rate-i;
				if (i%12==0)
				  gd_eval_led_toggle(LED1);
				delay_1ms(25);
			}
			for (i=rate+1;i>0;i=i-8){
				TIMER_CH0CV(TIMERxx)=i-1;
				TIMER_CH1CV(TIMERxx)=rate-i+1;
				TIMER_CH2CV(TIMERxx)=rate-i+1;
				if (i%12==0)
				  gd_eval_led_toggle(LED1);
				delay_1ms(25);
			}
			
        //if(RESET == gd_eval_key_state_get(KEY_WAKEUP)) {
            //gd_eval_led_on(LED1);
            //delay_1ms(500);
            //gd_eval_led_off(LED1);
            //delay_1ms(500);
        //}
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM1, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM1, USART_FLAG_TBE));

    return ch;
}
