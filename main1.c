#define YUZHI   12    //设定加热控制的阈值，PTC温度 > 设定值,停止加热，PTC温度 > (设定值-阈值) ,启动加热
#define TINGZHI  100  //摇动操作正转反转的停止时间系数
sbit DR_HEAT    at GPIOB_ODR.B12; //加热 GPIO2  PB12  1动 0停
sbit DR_PUMP    at GPIOB_ODR.B13;  //出水 GPIO1  PB13  1动 0停

sbit GPIO3    at GPIOB_ODR.B14;    //GPIO3  PB14  摇动控制 BI
sbit GPIO4    at GPIOB_ODR.B15;    //GPIO4  PB15  摇动控制 FI

sbit LED    at GPIOC_ODR.B13;  //LED指示灯  PC13
sbit BEEP    at GPIOA_ODR.B8;  //BEEP 蜂鸣器  PA8

//4G模块的控制引脚
sbit MCU_POWERKEY    at GPIOA_ODR.B15;  //  PA15
sbit MCU_RELOAD    at GPIOA_ODR.B12;  // PA12
sbit MCU_RESET    at GPIOA_ODR.B11;  // PA11

//红外接收的控制引脚       IR_ACT 状态读取
sbit IR_MOD    at GPIOB_ODR.B8;  // PB8


unsigned int oldstate1;
unsigned int oldstate2;
unsigned int oldstate3;

unsigned char flag1;
unsigned char flag_dingshi_chushui;
unsigned char flag_yaodong_shijian;
unsigned char flag3;
unsigned int countx1;
unsigned int countx2;

unsigned char flag_beep;
unsigned int couterx_beep;

unsigned int adc4_wendu_celiang;
unsigned int adc5_yaodong_shijian;
unsigned int adc6_wendu_sheding;
unsigned int adc7_chushui_shijian;


void YAODONG_MOTOR_CW(void)   //摇动奶瓶 正转
{
GPIO3=0;
GPIO4=1;
}

void YAODONG_MOTOR_CCW(void)    //摇动奶瓶 反转
{
GPIO3=1;
GPIO4=0;
}

void YAODONG_MOTOR_STOP(void)      //摇动奶瓶 停止
{
GPIO3=0;
GPIO4=0;
}


//Timer2 Prescaler :1; Preload = 39999; Actual Interrupt Time = 10 ms

//Place/Copy this part in declaration section
void InitTimer2(){
  RCC_APB1ENR.TIM2EN = 1;
  TIM2_CR1.CEN = 0;
  TIM2_PSC = 1;
  TIM2_ARR = 39999;
  NVIC_IntEnable(IVT_INT_TIM2);
  TIM2_DIER.UIE = 1;
  TIM2_CR1.CEN = 1;
}

void Timer2_interrupt() iv IVT_INT_TIM2 {
  TIM2_SR.UIF = 0;
  //按键声音的时间控制
  if(flag_beep==1)
  {
  couterx_beep++;
  }
  if(couterx_beep>20)  //按键时间为20倍的定时器时间，定时器为10ms 20*10=200ms
  {
  flag_beep=0;
  couterx_beep=0;
  }
  //Enter your code here
  if(flag_dingshi_chushui==1)   //定时出水标志位
  {
    countx1++;
  }
  if(flag_yaodong_shijian==1)
  {
    countx2++;
  }
  if(countx1>=adc7_chushui_shijian)   //判断定时出水结束时间  时间通过面板旋钮进行设定
  {  
  flag_dingshi_chushui=0;
  DR_PUMP=0; //关闭水泵
  }
  if(countx2>=adc5_yaodong_shijian)
  {
       YAODONG_MOTOR_STOP();//停止摇动
  }
  //TINGZHI
   if(countx2>=adc5_yaodong_shijian+TINGZHI)
  {
       YAODONG_MOTOR_CCW();//逆时针摇动开始
  }
  
  if(countx2>=adc5_yaodong_shijian*2)
  {
       YAODONG_MOTOR_STOP();//停止摇动
  }
  if(countx2>=adc5_yaodong_shijian*2+TINGZHI)
  {
       YAODONG_MOTOR_STOP();//停止摇动
       flag_yaodong_shijian=0;
       countx2=0;
  }
}


void main() {
flag_beep=0;
couterx_beep=0;
//set output
//LED指示灯  PC13
GPIO_Digital_Output(&GPIOC_ODR, _GPIO_PINMASK_13);
//BEEP 蜂鸣器  PA8
GPIO_Digital_Output(&GPIOA_ODR, _GPIO_PINMASK_8);
/*
sbit DR_HEAT    at GPIOB_ODR.B12; //加热 GPIO2  PB12  1动 0停
sbit DR_PUMP    at GPIOA_ODR.B8;  //出水 GPIO1  PB13  1动 0停

sbit GPIO3    at GPIOB_ODR.B14;    //GPIO3  PB14  摇动控制 BI
sbit GPIO4    at GPIOB_ODR.B15;    //GPIO4  PB15  摇动控制 FI
*/

GPIO_Digital_Output(&GPIOB_ODR, _GPIO_PINMASK_12);   //加热 GPIO2  PB12  1动 0停
GPIO_Digital_Output(&GPIOB_ODR, _GPIO_PINMASK_8);   //出水 GPIO1  PB13  1动 0停
GPIO_Digital_Output(&GPIOB_ODR, _GPIO_PINMASK_14);  //GPIO3  PB14  摇动控制 BI
GPIO_Digital_Output(&GPIOB_ODR, _GPIO_PINMASK_15);  //GPIO4  PB15  摇动控制 FI

//SET INPUT
// KEY1  PC14  定时出水
GPIO_Digital_Input(&GPIOC_IDR, _GPIO_PINMASK_14);
// KEY2  PC15 按下出水
GPIO_Digital_Input(&GPIOC_IDR, _GPIO_PINMASK_15);

//红外     IR_ACT PB7 状态读取
GPIO_Digital_Input(&GPIOB_IDR, _GPIO_PINMASK_7);


oldstate1 = 0;
oldstate2 = 0;

InitTimer2();
ADC_Set_Input_Channel(_ADC_CHANNEL_9 | _ADC_CHANNEL_5 |_ADC_CHANNEL_6  |_ADC_CHANNEL_7); // Set ADC channel 10 as an analog input
ADC1_Init();                            // Initialize ADC module

flag_yaodong_shijian=0;
UART1_Init(115200);  //4G 模块通讯
UART2_Init(9600);  //红外信号接收串口
//启动时，蜂鸣器响3声 ，可以设置蜂鸣器的声音长短
BEEP=1;
delay_ms(500); //延时500毫秒
BEEP=0;
BEEP=1;
delay_ms(500);
BEEP=0;
BEEP=1;
delay_ms(500);
BEEP=0;

while(1)
{
/*LED=1;
BEEP=1;
delay_ms(1000);
LED=0;
BEEP=0;
delay_ms(1000);*/
//read 4 adc
adc4_wendu_celiang = ADC1_Get_Sample(9); //OLD IS 4, NEW IS 9
adc5_yaodong_shijian = ADC1_Get_Sample(5);
adc6_wendu_sheding = ADC1_Get_Sample(6);
adc7_chushui_shijian = ADC1_Get_Sample(7); //出水时间
//对ADC的测试代码
/*if(adc4_wendu_celiang >500)
{
LED=1;
}
else
{
 LED=0;
}*/
if(flag_beep==1)
{
BEEP=1;
LED=1;
}
else
{
 BEEP=0;
 LED=0;
}

//温度控制逻辑
//启动和停止加热时，可以通过摇动来实现温度的更均衡
if(adc6_wendu_sheding>=adc4_wendu_celiang)  //温度设定值大于等于温度测量值
{
  DR_HEAT=0;//停止加热
}
if(adc6_wendu_sheding< adc4_wendu_celiang - YUZHI)  //温度设定值大于等于温度测量值
{
  DR_HEAT=1;//启动加热
}


//PUSH THE WATER TO THE BOTT   TIME BY ADC1
//按下出水  PC15  key1
  if (Button(&GPIOC_IDR, 15, 1, 1))
  {
  oldstate2 = 1;
  }

  if (Button(&GPIOC_IDR, 15, 1, 0))     //
  {
  if(oldstate2==1)
  {
   flag_beep=1;
      oldstate2=0;
  }
      DR_PUMP=1; //打开水泵
      flag_dingshi_chushui=0;//按下出水按钮，定时出水标志位清零
      if(flag_yaodong_shijian==0) //如果前一次摇动程序没有结束，不启动第二次摇动程序
      {
      flag_yaodong_shijian=1;//启动摇动程序
      countx2=0;
      }
      YAODONG_MOTOR_CW();//顺时针摇动开始
   }
   else
   {
    if(flag_dingshi_chushui==0)   //注意：没有按钮按下时，也运行程序，需要判断没有定时出水时，关闭水泵
    {
      DR_PUMP=0; //关闭水泵
    }
   }

//定时出水   PC14
  if (Button(&GPIOC_IDR, 14, 1, 0))
      oldstate1 = 1;
    if (oldstate1 && Button(&GPIOC_IDR, 14, 1, 1))
    {
      oldstate1 = 0;
      flag_dingshi_chushui=1;
      flag_beep=1;
      DR_PUMP=1; //打开水泵
      if(flag_yaodong_shijian==0) //如果前一次摇动程序没有结束，不启动第二次摇动程序
      {
      flag_yaodong_shijian=1;//启动摇动程序
      countx2=0;
      }
      YAODONG_MOTOR_CW();//顺时针摇动开始
      flag_dingshi_chushui=1;
      countx1=0;//通过计数器判断何时关闭水泵
    }

}
  

}
