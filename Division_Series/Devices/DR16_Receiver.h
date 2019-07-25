#ifndef __DR16_RECEIVER_H
#define __DR16_RECEIVER_H
/******************************************************************************/
#include "Users.h"
/******************************************************************************/
#define DR16_DBUS_PACKSIZE 		18u		//���ջ�����С

#define DR16_ROCKER_MAXVALUE 	660		//ң��ҡ�����ֵ

//���������¶�Ӧֵ
#define DR16_SWITCH_UP			1
#define DR16_SWITCH_MID			3
#define DR16_SWITCH_DOWN		2

#define MOUSEKEY_NONE 	0x00
#define MOUSEKEY_LEFT 	0x01
#define MOUSEKEY_RIGHT 	0x01

/*-------���̼�ֵ Begin------*/
#define KEYBOARD_PRESSED_NONE 	0x0000

#define KEYBOARD_PRESSED_W 		  0x0001
#define KEYBOARD_PRESSED_S 		  0x0002
#define KEYBOARD_PRESSED_A 		  0x0004
#define KEYBOARD_PRESSED_D 		  0x0008
#define KEYBOARD_PRESSED_SHIFT 	0x0010
#define KEYBOARD_PRESSED_CTRL 	0x0020
#define KEYBOARD_PRESSED_Q  	  0x0040
#define KEYBOARD_PRESSED_E 	  	0x0080

#define KEYBOARD_PRESSED_R 	  	0x0100
#define KEYBOARD_PRESSED_F 	  	0x0200
#define KEYBOARD_PRESSED_G 	  	0x0400
#define KEYBOARD_PRESSED_Z 	  	0x0800
#define KEYBOARD_PRESSED_X 	  	0x1000
#define KEYBOARD_PRESSED_C 	  	0x2000
#define KEYBOARD_PRESSED_V 	  	0x4000
#define KEYBOARD_PRESSED_B 	  	0x8000
/*-------���̼�ֵ End------*/

/*����*/
#define KEYMOUSE_Index       18
/*����̰�ʱ��*/
#define KeyMouse_Click_Time  1
/*���󳤰�ʱ��*/
#define KeyMouse_Long_Time   16
/******************************************************************************/

typedef struct {
	struct{
		/*����switch*/
		uint8_t s_left;
		uint8_t s_right;
		/*ҡ��4ͨ��*/
		int16_t ch0;
		int16_t ch1;
		int16_t ch2;
		int16_t ch3;
	}rc;
	
	struct{
		uint8_t keyLeft;
		uint8_t keyRight;
		
		int16_t x;
		int16_t y;
		int16_t z;
	}mouse;
	
	struct {
		uint16_t key_code;
	}keyBoard;
	
	/*����*/
	uint16_t thumbwheel;
	
	/*֡��*/
	uint16_t infoUpdateFrame;	
	
	/*�豸���߱�־*/
	uint8_t offLineFlag;

}DR16_DBUS_DATA_t;



typedef enum{
	KEY_W,
	KEY_S,
	KEY_A,
	KEY_D,
	KEY_SHIFT,
	KEY_CTRL,
	KEY_Q,
	KEY_E,
	KEY_R,
	KEY_F,
	KEY_G,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	MOUSE_LEFT,
	MOUSE_RIGHT,
}KeyMouseData_e;


typedef enum 
{
	Press,
	Click_Press,
	Long_Press,
}KeyMouseActionType;

/******************************************************************************/
extern DR16_DBUS_DATA_t 	DR16;
extern uint8_t 						DR16_rxBuff[DR16_DBUS_PACKSIZE+2];

/*����̰���־*/
extern uint32_t KeyMouse_Click_Press_Flag;
/*���󳤰���־*/
extern uint32_t KeyMouse_Long_Press_Flag;
/*�����±�־*/
extern uint32_t KeyMouse_Press_Flag;
/*�����¼�ʱ*/
extern uint32_t KeyMousePressTime[KEYMOUSE_Index];
/******************************************************************************/
void DR16_receiverInit(void);
bool DR16_receiverTest(void);
void DR16_processData(uint8_t *pData);
uint8_t DBUS_CheckPush(uint32_t Key);
void KeyMouseActionMonitor(void);
uint8_t GetKeyMouseAction(KeyMouseData_e KeyMouse, KeyMouseActionType Action);
bool KeyPRO(void);
/******************************************************************************/
#endif /* __DR16_RECEIVER_H */
