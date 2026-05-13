/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_aht20_interface_template.c
 * @brief     driver aht20 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-10-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/10/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "driver_aht20_interface.h"
#include "gd32l23x_i2c.h"
#include "systick.h"
static void aht20_interface_delay_us(uint32_t us);
/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t aht20_interface_iic_init(void)
{
    // 初始化 I2C1
    rcu_periph_clock_enable(RCU_I2C1);
    rcu_periph_clock_enable(RCU_GPIOB);

    //PB6,PB7通过AF4映射为I2C1
    gpio_af_set(GPIOB, GPIO_AF_8, GPIO_PIN_6);//I2C1_SCL
    gpio_af_set(GPIOB, GPIO_AF_8, GPIO_PIN_7);//I2C1_SDA

    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    
    /* configure I2C timing */
    // i2c_timing_config(I2C1, 0, 0x3, 0);
    // i2c_master_clock_config(I2C1, 0x13, 0x36);
    i2c_timing_config(I2C1, 0, 0, 0);
    uint32_t sclh = 80; // SCL high period
    uint32_t scll = 240; // SCL low period
    i2c_master_clock_config(I2C1, sclh, scll);
    
    /* configure I2C address */
    // i2c_address_config(I2C1, I2C_OWN_ADDRESS7, I2C_ADDFORMAT_7BITS);
    /* configure slave address */
    // i2c_master_addressing(I2C1, I2C_ADDFORMAT_7BITS, I2C_MASTER_TRANSMIT);
    /* configure number of bytes to be transferred */
    i2c_transfer_byte_number_config(I2C1, 2);
    /* enable I2C1 */
    i2c_enable(I2C1);   
    return 0;
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t aht20_interface_iic_deinit(void)
{
    
    // 禁用 I2C1
    i2c_disable(I2C1);
    return 0;
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t aht20_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t count=len;
     /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
     // 配置为接收模式
    i2c_master_addressing(I2C1, addr, I2C_MASTER_RECEIVE);
    // 配置要接收的字节数
    i2c_transfer_byte_number_config(I2C1, len);
    // 生成开始条件
    i2c_start_on_bus(I2C1);
    // 等待数据接收完毕
    // while(i2c_flag_get(I2C1, I2C_FLAG_RBNE) == RESET) {
    // }
    // 读取数据
    while(count--) {
        while(i2c_flag_get(I2C1, I2C_FLAG_RBNE) == RESET) {
        }
        *buf++ = i2c_data_receive(I2C1);
    }
    aht20_interface_delay_us(1);
    // 生成停止条件
    i2c_stop_on_bus(I2C1);
      /* wait until stop condition generate */
    while(!i2c_flag_get(I2C1, I2C_FLAG_STPDET));
    /* clear the STPDET bit */
    i2c_flag_clear(I2C1, I2C_FLAG_STPDET);
    return 0;
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t aht20_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
    uint16_t count=len;

     // 配置为发送模式
    i2c_master_addressing(I2C1, addr, I2C_MASTER_TRANSMIT);
    /* wait until I2C bus is idle */
    while(i2c_flag_get(I2C1, I2C_FLAG_I2CBSY));
    // 配置要发送的字节数
    i2c_transfer_byte_number_config(I2C1, count);
    // 生成开始条件
    i2c_start_on_bus(I2C1);

     /* wait until the transmit data buffer is empty */
    I2C_STAT(I2C1) |= I2C_STAT_TBE;
    while(!i2c_flag_get(I2C1, I2C_FLAG_TBE));

    // 发送数据
    while(count--) {
        
        i2c_data_transmit(I2C1, *buf++);
        // 等待可以发送数据
        while(i2c_flag_get(I2C1, I2C_FLAG_TBE) == RESET) {
        }
    }
    // 等待传输完成
    while(i2c_flag_get(I2C1, I2C_FLAG_TC) == RESET) {
    }
    // 生成停止条件
    i2c_stop_on_bus(I2C1);
      /* wait until stop condition generate */
    while(!i2c_flag_get(I2C1, I2C_FLAG_STPDET));
    /* clear the STPDET bit */
    i2c_flag_clear(I2C1, I2C_FLAG_STPDET);

    return 0;
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void aht20_interface_delay_ms(uint32_t ms)
{

    delay_1ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void aht20_interface_debug_print(const char *const fmt, ...)
{
    #include <stdio.h>
    #include <stdarg.h>
    va_list args;                    // 声明一个va_list变量
    va_start(args, fmt);              // 初始化va_list，fmt是最后一个固定参数

    vprintf(fmt, args);               // 使用vprintf来打印格式化输出

    va_end(args);                     // 清理va_list
}

static void aht20_interface_delay_us(uint32_t useconds)
{

    volatile uint32_t cycles = useconds * (SystemCoreClock / 1000000);
    while(cycles--) {
        __NOP();
    }
}
