#ifndef __LED_H
#define __LED_H
/******************************************************************************/
#include "Users.h"
//#include "stm32f4xx.h"
//#include "delay.h"
/******************************************************************************/
typedef enum{
	ON = 0,
	OFF = 1,
}LEDState_e;

typedef enum{
#if USE_BOARDTYPE == 0
	LED_D1 = 0,
	LED_D2,
	LED_D3,
	LED_D4,
	LED_D5,
	LED_D6,
	LED_D7,
	LED_D8,
	LED_D8_1,/*LED_NUM "+1"*/
#endif
	LED_GREEN,
	LED_RED,
}LEDMark_e;


typedef struct 
{
	GPIO_TypeDef*  GPIOx;
	uint32_t       GPIO_Pin;
	LEDState_e     LEDState;
}led_t;

/******************************************************************************/
/********************************
*led序列动作: 
*延时、停止、周期循环
*********************************/
#define LEDSEQ_WAITMS(X) (X)
#define LEDSEQ_STOP      -1
#define LEDSEQ_LOOP      -2

#define SEQ_NUM (sizeof(sequences)/sizeof(sequences[0]))

typedef struct 
{
	bool value;
	int action;
} ledseq_t;

extern const ledseq_t seq_DR16[];
extern const ledseq_t seq_YAW[];
extern const ledseq_t seq_PITCH[];
extern const ledseq_t seq_KRELOAD[];
extern const ledseq_t seq_GY_IMU[];
extern const ledseq_t seq_GY_IMU[];
extern const ledseq_t seq_NDOF_IMU[];
extern const ledseq_t seq_Ultrasonic_L[];
extern const ledseq_t seq_Ultrasonic_R[];
extern const ledseq_t seq_JudgeSystem[];
	
void ledseqInit(void);
bool ledseqTest(void);
void ledseqEnable(bool enable);
void ledseqRun(LEDMark_e led, const ledseq_t * sequence);
void ledseqStop(LEDMark_e led, const ledseq_t * sequence);
void ledseqSetTimes(ledseq_t *sequence, s32 onTime, s32 offTime);
bool ledTest(void);
/******************************************************************************/

#if USE_BOARDTYPE == 0
  #define  LED_NUM        10  //9+1 add "LED_D8_1" 
  #define  LED_ALL_CLK    LED_D8_1_GPIO_CLK|LED_GREEN_GPIO_CLK|LED_RED_GPIO_CLK
#else
  #define  LED_NUM         2
  #define  LED_ALL_CLK    LED_GREEN_GPIO_CLK|LED_RED_GPIO_CLK
#endif

#if USE_BOARDTYPE == 0
/*D1~D8*/
#define LED_D8_1_GPIO_CLK    RCC_AHB1Periph_GPIOG
#define LED_D8_1_GPIO_PORT   GPIOG
#define LED_D8_1_GPIO_PIN    LED_D1_PIN|LED_D2_PIN|LED_D3_PIN|LED_D4_PIN \
                             |LED_D5_PIN|LED_D6_PIN|LED_D7_PIN|LED_D8_PIN
/*D1*//*left-most*/
#define LED_D1_PIN           GPIO_Pin_1
/*D2*/
#define LED_D2_PIN           GPIO_Pin_2
/*D3*/
#define LED_D3_PIN           GPIO_Pin_3
/*D4*/
#define LED_D4_PIN           GPIO_Pin_4
/*D5*/
#define LED_D5_PIN           GPIO_Pin_5
/*D6*/
#define LED_D6_PIN           GPIO_Pin_6
/*D7*/
#define LED_D7_PIN           GPIO_Pin_7
/*D8*//*right-most*/
#define LED_D8_PIN           GPIO_Pin_8
#endif


#define LED_GREEN_GPIO_CLK    RCC_AHB1Periph_GPIOF
#define LED_GREEN_PIN         GPIO_Pin_14
#define LED_GREEN_GPIO_PORT   GPIOF


#define LED_RED_GPIO_CLK      RCC_AHB1Periph_GPIOE
  #if USE_BOARDTYPE == 0
  #define LED_RED_PIN           GPIO_Pin_11
  #else
	#define LED_RED_PIN           GPIO_Pin_7
  #endif
#define LED_RED_GPIO_PORT     GPIOE



#define LED_GREEN_TOGGLE() 		GPIO_ToggleBits(LED_GREEN_GPIO_PORT,LED_GREEN_PIN)
#define LED_RED_TOGGLE() 			GPIO_ToggleBits(LED_RED_GPIO_PORT, LED_RED_PIN)
/******************************************************************************/

void LED_Init(void);
void LED_State(void);
void LedSet(LEDMark_e led, LEDState_e status);
void LedFlashOne(LEDMark_e led, u32 onTime, u32 offTime);
void LedAllON(void);
void LedAllOFF(void);
/******************************************************************************/

#endif /* __LED_H */

