/*!
    \file    main.c
    \brief   led spark with systick, USART print and key example

    \version 2017-06-06, V1.0.0, firmware for GD32F3x0
    \version 2019-06-01, V2.0.0, firmware for GD32F3x0
    \version 2020-09-30, V2.1.0, firmware for GD32F3x0
    \version 2022-01-06, V2.2.0, firmware for GD32F3x0
*/

#include "gd32f3x0.h"
#include "systick.h"
#include <stdio.h>
#include "main.h"
#include "gd32f310r_eval.h"



/*!
    \brief      toggle the led every 500ms
    \param[in]  none
    \param[out] none
    \retval     none
*/
void led_spark(void)
{
    static __IO uint32_t timingdelaylocal = 0U;

    if(timingdelaylocal) {

        if(timingdelaylocal < 500U) {
            gd_eval_led_on(LED1);
        } else {
            gd_eval_led_off(LED1);
        }

        timingdelaylocal--;
    } else {
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
    /* configure systick */
    systick_config();
    /* initilize the LEDs, USART and key */
    gd_eval_led_init(LED1);
    gd_eval_led_init(LED2);
    gd_eval_led_init(LED3);
    gd_eval_com_init(EVAL_COM);
    gd_eval_key_init(KEY_WAKEUP, KEY_MODE_GPIO);

    /* print out the clock frequency of system, AHB, APB1 and APB2 */
    printf("\r\nCK_SYS is %d", rcu_clock_freq_get(CK_SYS));
    printf("\r\nCK_AHB is %d", rcu_clock_freq_get(CK_AHB));
    printf("\r\nCK_APB1 is %d", rcu_clock_freq_get(CK_APB1));
    printf("\r\nCK_APB2 is %d", rcu_clock_freq_get(CK_APB2));

    
    uint32_t dummy = 0;
    timer_parameter_struct timer_initpara;
    timer_oc_parameter_struct timer_ocintpara;
    timer_break_parameter_struct timer_bkdtpara;
    
    /************************************* step0: 开启时钟 *******************************************/
    rcu_periph_clock_enable(RCU_GPIOA);
    rcu_periph_clock_enable(RCU_GPIOB);
    rcu_periph_clock_enable(RCU_TIMER0);
    rcu_periph_clock_enable(RCU_ADC);
    rcu_adc_clock_config(RCU_ADCCK_APB2_DIV2);
    
    /************************************* step1: GPIO配置 *******************************************/
    /* 配置6路PWM的IO */
    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_8);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_8);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_8);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_9);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_9);

    gpio_mode_set(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_10);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    gpio_af_set(GPIOA, GPIO_AF_2, GPIO_PIN_10);

    /* 驱动使能 */
    gpio_mode_set(GPIOA, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_5);
    gpio_output_options_set(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_5);
    gpio_bit_reset(GPIOA, GPIO_PIN_5);
    
    /* 配置电流采样的IO */
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_1);
    gpio_mode_set(GPIOA, GPIO_MODE_ANALOG, GPIO_PUPD_NONE, GPIO_PIN_2);


    /************************************* step2: timer配置 ******************************************/
    timer_deinit(TIMER0);
    timer_initpara.prescaler = PWM_PRSC;
    timer_initpara.alignedmode = TIMER_COUNTER_CENTER_DOWN;
    timer_initpara.period = HALF_PWM_PERIOD;          /* 配置三相桥开关频率 */
    timer_initpara.clockdivision = TIMER_CKDIV_DIV2;  /* 死区时间计算用到的分频 */
    timer_initpara.repetitioncounter = 1;                /* 重复计数器, 影响更新事件的频率 */
    timer_init(TIMER0, &timer_initpara);
    
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.outputnstate = TIMER_CCXN_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;     
    timer_ocintpara.ocnpolarity = TIMER_OCN_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_ocintpara.ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
    
    timer_channel_output_config(TIMER0, TIMER_CH_0,&timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_1,&timer_ocintpara);
    timer_channel_output_config(TIMER0, TIMER_CH_2,&timer_ocintpara);
    
    timer_ocintpara.outputstate = TIMER_CCX_ENABLE;
    timer_ocintpara.ocpolarity = TIMER_OC_POLARITY_HIGH;
    timer_ocintpara.ocidlestate = TIMER_OC_IDLE_STATE_LOW;
    timer_channel_output_config(TIMER0, TIMER_CH_3,&timer_ocintpara);
    
    /* channel1~3 */
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_0, HALF_PWM_PERIOD/2);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_0, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_0, TIMER_OC_SHADOW_ENABLE);   /* 预装载使能 */
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_1, HALF_PWM_PERIOD/2);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_1, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_1, TIMER_OC_SHADOW_ENABLE);   /* 预装载使能 */
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_2, HALF_PWM_PERIOD/2);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_2, TIMER_OC_MODE_PWM1);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_2, TIMER_OC_SHADOW_ENABLE);   /* 预装载使能 */
    
    /* channel4 设置, 触发ADC用 */
    timer_channel_output_pulse_value_config(TIMER0, TIMER_CH_3, 5);
    timer_channel_output_mode_config(TIMER0, TIMER_CH_3, TIMER_OC_MODE_PWM0);
    timer_channel_output_shadow_config(TIMER0, TIMER_CH_3, TIMER_OC_SHADOW_ENABLE);   /* 预装载使能 */
    timer_master_output_trigger_source_select(TIMER0, TIMER_TRI_OUT_SRC_O3CPRE);      /* 上升沿触发ADC */
    
    /* 配置死区 */
    timer_bkdtpara.runoffstate = TIMER_ROS_STATE_ENABLE;
    timer_bkdtpara.ideloffstate = TIMER_IOS_STATE_ENABLE;
    timer_bkdtpara.protectmode = TIMER_CCHP_PROT_0;
    timer_bkdtpara.deadtime = DEADTIME;
    timer_bkdtpara.breakstate = TIMER_BREAK_DISABLE;
    timer_bkdtpara.breakpolarity = TIMER_BREAK_POLARITY_LOW;
    timer_bkdtpara.outputautostate = TIMER_OUTAUTO_DISABLE;
    timer_break_config(TIMER0, &timer_bkdtpara);

    /* 多电机控制同步用的 */
    timer_input_trigger_source_select(TIMER0, TIMER_SMCFG_TRGSEL_ITI1);
    timer_slave_mode_select(TIMER0, TIMER_SLAVE_MODE_EVENT);
    timer_event_software_generate(TIMER0, TIMER_EVENT_SRC_UPG);
    timer_counter_value_config(TIMER0, 0);

    /* 只需要CC4使能 */
    timer_channel_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCX_DISABLE);
    timer_channel_output_state_config(TIMER0, TIMER_CH_3, TIMER_CCX_ENABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_0, TIMER_CCXN_DISABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_1, TIMER_CCXN_DISABLE);
    timer_channel_complementary_output_state_config(TIMER0, TIMER_CH_2, TIMER_CCXN_DISABLE);

    timer_update_source_config(TIMER0, TIMER_UPDATE_SRC_REGULAR);
    timer_single_pulse_mode_config(TIMER0, TIMER_SP_MODE_REPETITIVE);
    timer_primary_output_config(TIMER0,ENABLE);
    timer_auto_reload_shadow_enable(TIMER0);
    timer_enable(TIMER0);

    dbg_periph_enable(DBG_TIMER0_HOLD);

    /************************************** step3: adc配置 *******************************************/
    adc_deinit();
    adc_special_function_config(ADC_SCAN_MODE, ENABLE);
    adc_special_function_config(ADC_CONTINUOUS_MODE, DISABLE);
    adc_external_trigger_source_config(ADC_REGULAR_CHANNEL, ADC_EXTTRIG_REGULAR_NONE); 
    adc_data_alignment_config(ADC_DATAALIGN_RIGHT);

    /* 注入组配置 */
    adc_channel_length_config(ADC_INSERTED_CHANNEL, 2);
    adc_inserted_channel_config(0, ADC_CHANNEL_1, ADC_SAMPLETIME_7POINT5);         /* A相电流 */
    adc_inserted_channel_config(1, ADC_CHANNEL_2, ADC_SAMPLETIME_7POINT5);         /* B相电流 */

    adc_external_trigger_source_config(ADC_INSERTED_CHANNEL, ADC_EXTTRIG_INSERTED_T0_CH3); 
    adc_external_trigger_config(ADC_INSERTED_CHANNEL, ENABLE);

    adc_enable();
    for(dummy = 0; dummy < 60000; dummy++);
    adc_calibration_enable();
    
    adc_interrupt_enable(ADC_INT_EOIC);
    nvic_irq_enable(ADC_CMP_IRQn, ADC_PRE_PRIORITY, ADC_SUB_PRIORITY);
    
    
    while(1) {
        if(RESET == gd_eval_key_state_get(KEY_WAKEUP)) {
            gd_eval_led_on(LED2);
            delay_1ms(500);
            gd_eval_led_off(LED2);
            gd_eval_led_toggle(LED3);
        }
    }
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(EVAL_COM, (uint8_t)ch);
    while(RESET == usart_flag_get(EVAL_COM, USART_FLAG_TBE));

    return ch;
}
