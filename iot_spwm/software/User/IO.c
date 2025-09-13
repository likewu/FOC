/******************************************************************************
* Copyright 2014-2015  Co.,Ltd.
* FileName: 	 IO.C
* Desc: IO的操作
*
*
* Author:
* Date:
* Notes:
*
* -----------------------------------------------------------------
* Histroy:
*
******************************************************************************/

/*-------------------------------- Includes ----------------------------------*/
#include "IO.h"

/*-------------------- Global Definitions and Declarations -------------------*/


/*----------------------- Constant / Macro Definitions -----------------------*/


/*------------------------ Type Declarations ---------------------------------*/
GPIO_TypeDef *g_io_GPIORegsTab[]=
{
    GPIOA,
    GPIOB,
    GPIOC,
    GPIOD,
    GPIOE,
    GPIOF,
    GPIOG,
};

/*------------------------ Variable Declarations -----------------------------*/


/*------------------------ Function Prototype --------------------------------*/


/*------------------------ Function Implement --------------------------------*/


/*----------------------------------------------------------------------------*/

/******************************************************************************
* Name:    SET_IO_OUT_OPEN_DRAIN()                          *
* Desc:   IO设置为开漏输出                           *
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/

void SET_IO_OUT_OPEN_DRAIN(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x06 << lpin);
    *reg = value;

}
/******************************************************************************
* Name:    SET_IO_OUT()                          *
* Desc:   IO设置为推挽输出                           *
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_OUT_ISR(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x02 << lpin);
    *reg = value;

}
void SET_IO_OUT(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x02 << lpin);
    *reg = value;

}
void SET_IO_OUT_HS(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x03 << lpin);
    *reg = value;

}

void SET_IO_SET_BIT(GPIO_TypeDef *port, U8 pin)
{
    port->BSRR = ((U32)0x01<<pin);
}
/******************************************************************************
* Name:    SET_IO_LOW()                          				*
* Desc:     推挽输出低                      		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_RESET_BIT(GPIO_TypeDef *port, U8 pin)
{
    port->BRR = ((U32)0x01<<pin);
}

void SET_IO_IN_WITHOUTPULLUP(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x04 << lpin);
    *reg = value;
}
void SET_IO_IN_PULLUP(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    port->BSRR = ((U32)0x01<<pin);
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x08 << lpin);
    *reg = value;
}
/******************************************************************************
* Name:    SET_IO_IN()                          				*
* Desc:   IO设置为输入                         		 *
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_ANALOG(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x00 << lpin);
    *reg = value;

}



/******************************************************************************
* Name:    SET_IO_AFMODE()                          				*
* Desc:                            		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_AFMODE_PP(GPIO_TypeDef *port, U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x0A << lpin);
    *reg = value;

}
void SET_IO_AFMODE_PP_HS(GPIO_TypeDef *port, U8 pin)
{
    U32 value;
    volatile U32 *reg;
    U8 lpin;
    if(pin < 8)
    {
        lpin = pin * 4;
        reg = &port->CRL;
    }
    else
    {
        lpin = (pin - 8) * 4;
        reg = &port->CRH;
    }
    value = *reg;
    value &= ~((U32)0x0F << lpin);
    value |= ((U32)0x0B << lpin);
    *reg = value;

}

/******************************************************************************
* Name:    SET_IO_HIGH()                          				*
* Desc:     推挽输出高                       		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_HIGH(GPIO_TypeDef *port, U8 pin)
{
    port->BSRR = ((U32)0x01<<pin);
    SET_IO_OUT(port,pin);
    port->BSRR = ((U32)0x01<<pin);
}
/******************************************************************************
* Name:    SET_IO_LOW()                          				*
* Desc:     推挽输出低                      		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_LOW(GPIO_TypeDef *port, U8 pin)
{
    port->BRR = ((U32)0x01<<pin);
    SET_IO_OUT(port,pin);
    port->BRR = ((U32)0x01<<pin);
}
/******************************************************************************
* Name:    SET_IO_OPEN_HIGH()                          				*
* Desc:      开漏输出高                      		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_HIGH_OPEN_DRAIN(GPIO_TypeDef *port, U8 pin)
{
    port->BSRR = ((U32)0x01<<pin);
    SET_IO_OUT_OPEN_DRAIN(port,pin);
    port->BSRR = ((U32)0x01<<pin);
}
/******************************************************************************
* Name:    SET_IO_LOW_OPEN_DRAIN()                          				*
* Desc:      开漏输出低                     		 					*
* Param:                                                                      *
* Return:                                                                     *
* Global:                                                                     *
* Note:                                                                       *
* Author:                                                                     *
* Log:                                                                        *
*******************************************************************************/
void SET_IO_LOW_OPEN_DRAIN(GPIO_TypeDef *port,U8 pin)
{
    port->BRR = ((U32)0x01<<pin);
    SET_IO_OUT_OPEN_DRAIN(port,pin);
    port->BRR = ((U32)0x01<<pin);
}

void SET_IO_TOGGLE(GPIO_TypeDef *port,U8 pin)
{
    U32 value;
    value = ((U32)0x01<<pin);
    if(value & port->ODR) {
        port->BRR = value;
    } else {
        port->BSRR = value;
    }

    SET_IO_OUT(port,pin);
}
U8 GET_IO_STA(GPIO_TypeDef *port,U8 pin)
{
    if (port->IDR & ((U32)0x01<<pin))
    {
        return HIGH;
    }
    else
    {
        return  LOW;
    }
}
U8 GET_IO_OUTSTA(GPIO_TypeDef *port,U8 pin)
{
    if (port->ODR & ((U32)0x01<<pin))
    {
        return HIGH;
    }
    else
    {
        return  LOW;
    }
}

