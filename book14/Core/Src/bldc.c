#include "bldc.h"
#include "stdlib.h"

BldcMotor_t mymotor;


// 用于得到HALL传感器位置
uint8_t GetBldcHall(void)
{
	uint8_t ret = 0;
	
	ret = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15);
	ret <<=1;
	ret |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3);
	ret <<=1;
	ret |= HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_10);
	
	return ret;
}


// 120 度HAL,如果你的电机直接用这个表驱动有问，那你可能需要根据
// 实际电机进行适当的调整对应关系，可在该表基础上进行适当修改即可
// 正 转
// 霍尔#1 霍尔#2 霍尔#3		A+ 		A- 		 B+ 		B- 		C+ 		C- 		方向
// 1 				0 		1 		关闭 		关闭 	关闭 	开通 	开通 		关闭 		↓
// 0 				0 		1 		关闭 		开通 	关闭 	关闭 	开通 		关闭 		↓
// 0 				1 		1 		关闭 		开通 	开通 	关闭 	关闭 		关闭 		↓
// 0 				1 		0 		关闭 		关闭 	开通 	关闭 	关闭 		开通 		↓
// 1 				1 		0 		开通 		关闭 	关闭 	关闭 	关闭 		开通 		↓
// 1 				0 		0 		开通 		关闭 	关闭 	开通 	关闭 		关闭 		↓

// 反 转
// 霍尔#1 霍尔#2 霍尔#3 	A+ 		A- 			B+ 		B- 		C+ 		C- 		方向
// 1 				0 		1 		关闭 		关闭 	开通 	关闭 	关闭 		开通 		↑
// 0 				0 		1 		开通 		关闭 	关闭 	关闭 	关闭 		开通 		↑
// 0 				1 		1 		开通 		关闭 	关闭 	开通 	关闭 		关闭 		↑
// 0 				1 		0 		关闭 		关闭 	关闭 	开通 	开通 		关闭 		↑
// 1 				1 		0 		关闭 		开通 	关闭 	关闭 	开通 		关闭 		↑
// 1 				0 		0 		关闭 		开通 	开通 	关闭 	关闭 		关闭 		↑


//  电机驱动
void BldcMove( BldcMotor_t *motor )
{
  return ;

	static uint8_t prehall = 0;
	motor->Hall = GetBldcHall();
	uint16_t Speed;
	
	motor->Dir = motor->PwmRef > 0 ? 1 : 0;
	
	Speed = abs(motor->PwmRef);
	if( Speed > 1199 ) motor->PwmRef = 1199;
	
	if( motor->EN )
	{
		TIM1->CCR1 = Speed;
		TIM1->CCR2 = Speed;
		TIM1->CCR3 = Speed;
	}
	else
	{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = 0;
		TIM1->CCR3 = 0;
		return ;
	}
	
	if( prehall != motor->Hall )
	{
		
		GPIOC->ODR &= ~(7<<10);
		
		if( motor->Dir )
		{
			switch( motor->Hall )
			{
				case 2:
					TIM1->CCER = 0x010;
					GPIOC->ODR |= 6<<10;
					break;

				case 6:
					TIM1->CCER = 0x001;
					GPIOC->ODR |= 5<<10;
					break;

				case 4:
					TIM1->CCER = 0x001;
					GPIOC->ODR |= 3<<10;
					break;

				case 5:
					TIM1->CCER = 0x100;
					GPIOC->ODR |= 6<<10;
					break;

				case 1:
					TIM1->CCER = 0x100;
					GPIOC->ODR |= 5<<10;
					break;

				case 3:
					TIM1->CCER = 0x010;
					GPIOC->ODR |= 3<<10;
					break;
				
				default:break;
			}
		}
		else
		{
			switch( motor->Hall )
			{
				case 5:
					TIM1->CCER = 0x010;
					GPIOC->ODR |= 6<<10;
					break;

				case 1:
					TIM1->CCER = 0x001;
					GPIOC->ODR |= 5<<10;
					break;

				case 3:
					TIM1->CCER = 0x001;
					GPIOC->ODR |= 3<<10;
					break;

				case 2:
					TIM1->CCER = 0x100;
					GPIOC->ODR |= 6<<10;
					break;

				case 6:
					TIM1->CCER = 0x100;
					GPIOC->ODR |= 5<<10;
					break;

				case 4:
					TIM1->CCER = 0x010;
					GPIOC->ODR |= 3<<10;
					break;

				default:break;
			}
		}
		prehall = motor->Hall;
	}
	
}

//  电机驱动(简单版)
void BldcMove1( uint8_t Dir )
{
    static uint8_t prehall = 0;
    uint8_t hall = GetBldcHall();

    if( prehall != hall )
    {

        GPIOC->ODR &= ~(7<<10);
        GPIOA->ODR &= ~(7<<8);

        if( Dir )
        {
            switch( hall )
            {
                case 2:
                    GPIOA->ODR |= 2<<8;
                    GPIOC->ODR |= 6<<10;
                    break;

                case 6:
                    GPIOA->ODR |= 1<<8;
                    GPIOC->ODR |= 5<<10;
                    break;

                case 4:
                    GPIOA->ODR |= 1<<8;
                    GPIOC->ODR |= 3<<10;
                    break;

                case 5:
                    GPIOA->ODR |= 4<<8;
                    GPIOC->ODR |= 6<<10;
                    break;

                case 1:
                    GPIOA->ODR |= 4<<8;
                    GPIOC->ODR |= 5<<10;
                    break;

                case 3:
                    GPIOA->ODR |= 2<<8;
                    GPIOC->ODR |= 3<<10;
                    break;

                default:break;
            }
        }
        else
        {
            switch( hall )
            {
                case 5:
                    GPIOA->ODR |= 2<<8;
                    GPIOC->ODR |= 6<<10;
                    break;

                case 1:
                    GPIOA->ODR |= 1<<8;
                    GPIOC->ODR |= 5<<10;
                    break;

                case 3:
                    GPIOA->ODR |= 1<<8;
                    GPIOC->ODR |= 3<<10;
                    break;

                case 2:
                    GPIOA->ODR |= 4<<8;
                    GPIOC->ODR |= 6<<10;
                    break;

                case 6:
                    GPIOA->ODR |= 4<<8;
                    GPIOC->ODR |= 5<<10;
                    break;

                case 4:
                    GPIOA->ODR |= 2<<8;
                    GPIOC->ODR |= 3<<10;
                    break;

                default:break;
            }
        }
        prehall = hall;
    }
}

//  电机驱动
void BldcStart( BldcMotor_t *motor,int16_t speed )
{
	TIM_SlaveConfigTypeDef SlaveConfig;
	SlaveConfig.SlaveMode =  TIM_SLAVEMODE_DISABLE;
	SlaveConfig.InputTrigger = TIM_TS_TI1F_ED;
	SlaveConfig.TriggerPolarity = TIM_TRIGGERPOLARITY_BOTHEDGE;
	HAL_TIM_SlaveConfigSynchronization(&htim2, &SlaveConfig);
	 
  HAL_TIM_IC_Start(&htim2,TIM_CHANNEL_1);
	__HAL_TIM_ENABLE(&htim2);
	
	
	motor->PwmRef = speed;
	motor->EN = 1;
	BldcMove( motor );
	
	TIM1->BDTR |= TIM_BDTR_MOE;
	TIM1->CR1  |= TIM_CR1_CEN;
}


//  电机驱动
void BldcStop( BldcMotor_t *motor )
{
	motor->PwmRef = 0;
	motor->EN = 0;
	BldcMove( motor );
}



//  更新PWM值
void BldcUpdataPwm( int16_t pwm )
{
	uint16_t speed = abs(pwm);
	mymotor.PwmRef = pwm;
	mymotor.Dir = mymotor.PwmRef > 0 ? 1 : 0;

	TIM1->CCR1 = speed;
	TIM1->CCR2 = speed;
	TIM1->CCR3 = speed;
}




//  得到BLDC速度,以一对极电机为例，电机转一圈，产生6个HALL状态，每个状态对应60度机械角度
//  speed = 60 / 360 /t(s) (转/秒) =  60 * (60 /360) / t = 10/t(s) 转/分  = 10000000 / t(us)
//  上式如果是两对极电机，speed = 30 / 360 /t(s) (转/秒) =  30 * (60 /360) / t = 5/t(s) 转/分  = 5000000 / t(us)  
//  本例所用电机为2对极电机

int32_t GetBldcSpeed( BldcMotor_t *bldc, uint16_t tus )
{
	int32_t speed;
	
	// 数据极端处理
	if( (bldc->HallSpanTime + tus) > 1000000000 ) bldc->HallSpanTime = 1000000000;
	else bldc->HallSpanTime += tus;
	
	// 速度计算，直接得到转/分
	speed = 5000000/bldc->HallSpanTime;
	
	if( bldc->Dir == 0 )
		speed = -speed;
	
	// 一阶滞后滤波
	// bldc->SpeedBck = ( bldc->SpeedBck + speed )/2;
	bldc->SpeedBck = ( bldc->SpeedBck*8 + speed*2 )/10;
	// bldc->SpeedBck = ( bldc->SpeedBck*98 + speed*2 )/100;
	
	
	if( bldc->HallGet )
		bldc->HallSpanTime = 0;
	
}



