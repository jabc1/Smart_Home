#include "PWM.h"



/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void NVIC_Config_PWM_TIM4(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Configure one bit for preemption priority */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	/* 配置TIM4_IRQ中断为中断源 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//  舵机
//  TIM4 PWM部分初始化 
//  PWM输出初始化
//  arr：自动重装值
//  psc：时钟预分频数
void Servo_Door_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //基本定时器的结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //通道结构体的设置
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);           //使能GPIO外设和AFIO复用功能模块时钟
	
	
   //设置该引脚为复用输出功能,输出TIM4 CH4的PWM脉冲波形	GPIOB.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;                       // 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //初始化GPIO

   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel4 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性高
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);                        //根据T指定的参数初始化外设TIM4 OC4

	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);               //使能TIM4在CCR4上的预装载寄存器
 
	NVIC_Config_PWM_TIM4();
	TIM_Cmd(TIM4, ENABLE);                                          //使能TIM4
	TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Servo_Window_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //基本定时器的结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //通道结构体的设置
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);                   // 定时器重映射
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);           //使能GPIO外设和AFIO复用功能模块时钟
	
	
   //设置该引脚为复用输出功能,输出TIM4 CH3的PWM脉冲波形	GPIOD.14
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;                      //LED0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOD, &GPIO_InitStructure);                          //初始化GPIO

   //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = psc;                      //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel3 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;               //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);                        //根据T指定的参数初始化外设TIM4 OC3
	
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);               //使能TIM4在CCR3上的预装载寄存器
 
	NVIC_Config_PWM_TIM4();
	TIM_Cmd(TIM4, DISABLE);                                          //使能TIM4
	//TIM_CtrlPWMOutputs(TIM4, ENABLE);
}

void Fan_Init(u16 arr,u16 psc)                                      // (255,1999) 定时器从0计数到255，即为266次，为一个定时周期
{   
	GPIO_InitTypeDef GPIO_InitStructure;                            //GPIO结构体
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;                 //基本定时器的结构体
	TIM_OCInitTypeDef  TIM_OCInitStructure;                         //通道结构体的设置
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE);                   // 定时器重映射
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	        //使能定时器2时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);           //使能GPIO外设和AFIO复用功能模块时钟
	
	
   //设置该引脚为复用输出功能,输出TIM4 CH2的PWM脉冲波形	GPIOD.13
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                       //LED0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                 //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;           
	GPIO_Init(GPIOD, &GPIO_InitStructure);                          //初始化GPIO

    //初始化TIM4
	TIM_TimeBaseStructure.TIM_Period = arr;                         //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                       //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;     //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //选择定时器模式:TIM脉冲宽度调制模式1
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //比较输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;								//设置初始PWM脉冲宽度为0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;        //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);                        //根据T指定的参数初始化外设TIM4 OC2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);               //使能TIM4在CCR2上的预装载寄存器
 
    TIM_ARRPreloadConfig(TIM4, ENABLE);			 					//能TIM4重载寄存器ARR
    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);						//使能update中断
    
    NVIC_Config_PWM_TIM4();  
	TIM_Cmd(TIM4, DISABLE);                                          //使能TIM4
}

