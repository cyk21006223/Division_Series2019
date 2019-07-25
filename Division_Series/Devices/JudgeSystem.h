#ifndef __JUDGESYSTEM_H
#define __JUDGESYSTEM_H

/******************************************************************************/
#include "Users.h"
/*****************************内容不可修改*************************************/
#define Judge_BuffSIZE 	        300u			//裁判系统缓冲区长度

#define JudgeFrameLength_001    3+TotalLengthExceptData
#define JudgeFrameLength_002    1+TotalLengthExceptData
#define JudgeFrameLength_003    2+TotalLengthExceptData
#define JudgeFrameLength_101    4+TotalLengthExceptData
#define JudgeFrameLength_102    3+TotalLengthExceptData
#define JudgeFrameLength_103    2+TotalLengthExceptData
#define JudgeFrameLength_201    15+TotalLengthExceptData
#define JudgeFrameLength_202    14+TotalLengthExceptData
#define JudgeFrameLength_203    16+TotalLengthExceptData
#define JudgeFrameLength_204    1+TotalLengthExceptData
#define JudgeFrameLength_205    3+TotalLengthExceptData
#define JudgeFrameLength_206    1+TotalLengthExceptData
#define JudgeFrameLength_207    6+TotalLengthExceptData

#define ClientCustomLength      13+InteractiveHeaderLength+TotalLengthExceptData
#define RobotInteractionLength  RobotInteractBuffLength+InteractiveHeaderLength \
                                +TotalLengthExceptData

/*RM2019裁判系统串口协议附录V1.1,P4表2帧头详细定义*/
#define JudgeFrameHeaderSOF     0xA5                              //裁判系统帧头
#define JudgeFrameHeaderLength   5                            //裁判系统帧头长度
#define JudgeCmdIDLength         2                           //裁判系统CmdID长度
#define JudgeFrameTailLength     2                            //裁判系统帧尾长度
#define DataOffsetFromSOF       JudgeFrameHeaderLength \
                                +JudgeCmdIDLength            //Data的偏移量7字节
																
#define TotalLengthExceptData    JudgeFrameHeaderLength \
                                +JudgeCmdIDLength \
																+JudgeFrameTailLength  //裁判系统除Data以外9字节
																
#define InteractiveHeaderLength  6                 //裁判系统交互数据头结构6字节
#define RobotInteractBuffLength  1        //机器人间通信真实内容数据长度,小于113

/*****************************内容不可修改*************************************/

#define InteractiveDataID       0x0265        //参赛队自定义机器人交互数据内容ID
                                              //内容ID:0x0200~0x02FF
/*****************************内容不可修改*************************************/
//比赛状态数据
typedef __packed struct
{
uint8_t game_type : 4;
uint8_t game_progress : 4;
uint16_t stage_remain_time;
} ext_game_state_t;

//比赛结果数据
typedef __packed struct
{
  uint8_t winner;
} ext_game_result_t;


//机器人存活数据
typedef __packed struct
{
  uint16_t robot_legion;
} ext_game_robot_survivors_t;


//场地事件数据
typedef __packed struct
{
  uint32_t event_type;
} ext_event_data_t;

//补给站动作标识
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//补给站预约子弹
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_num;
} ext_supply_projectile_booking_t;

//比赛机器人状态
typedef __packed struct
{
uint8_t robot_id;
uint8_t robot_level;
uint16_t remain_HP;
uint16_t max_HP;
uint16_t shooter_heat0_cooling_rate;
uint16_t shooter_heat0_cooling_limit;
uint16_t shooter_heat1_cooling_rate;
uint16_t shooter_heat1_cooling_limit;
uint8_t mains_power_gimbal_output : 1;
uint8_t mains_power_chassis_output : 1;
uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

//实时功率热量数据
typedef __packed struct
{
  uint16_t chassis_volt;
  uint16_t chassis_current;
	float chassis_power;
  uint16_t chassis_power_buffer;
  uint16_t shooter_heat0;//17mm枪口热量
  uint16_t shooter_heat1;
} ext_power_heat_data_t;

//机器人位置
typedef __packed struct
{
  float x;
  float y;
  float z;
  float yaw;
} ext_game_robot_pos_t;

//机器人增益
typedef __packed struct
{
  uint8_t power_rune_buff;
}ext_buff_musk_t;

//空中机器人能量状态
typedef __packed struct
{
  uint8_t energy_point;
  uint8_t attack_time;
} aerial_robot_energy_t;

//伤害数据
typedef __packed struct
{
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;


//实时射击信息
typedef __packed struct
{
  uint8_t bullet_type;
  uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;


//交互数据接收信息
typedef __packed struct
{
uint16_t data_cmd_id;
uint16_t send_ID;
uint16_t receiver_ID;
}ext_student_interactive_header_data_t;


//客户端自定义数据
typedef struct
{
	union
	{
	 uint8_t databuff[13];
		__packed struct
		{
			float data1;
			float data2;
			float data3;
			uint8_t mask;
		};
	}data;
}client_custom_data_t;

//机器人间通信
typedef struct
{
	union
	{
   uint8_t data[RobotInteractBuffLength];
		__packed struct
		{
			uint8_t sign;
		};
	};
} robot_interactive_data_t;

typedef struct
{
  uint8_t Red_Hero;
	uint8_t Red_Engineer;
	uint8_t Red_Standard_3;
	uint8_t Red_Standard_4;
	uint8_t Red_Standard_5;
	uint8_t Red_Aerial;
	uint8_t Red_Sentry;
	
	uint8_t Blue_Hero;
	uint8_t Blue_Engineer;
	uint8_t Blue_Standard_3;
	uint8_t Blue_Standard_4;
	uint8_t Blue_Standard_5;
	uint8_t Blue_Aerial;
	uint8_t Blue_Sentry;
}robot_id_t;

typedef struct
{
  uint16_t Red_Hero;
	uint16_t Red_Engineer;
	uint16_t Red_Standard_3;
	uint16_t Red_Standard_4;
	uint16_t Red_Standard_5;
	uint16_t Red_Aerial;
	
	uint16_t Blue_Hero;
	uint16_t Blue_Engineer;
	uint16_t Blue_Standard_3;
	uint16_t Blue_Standard_4;
	uint16_t Blue_Standard_5;
	uint16_t Blue_Aerial;
}client_id_t;
/*****************************内容不可修改*************************************/

//裁判系统帧率
typedef  struct{
	int32_t  BulletLaunched;  //已发射弹量
	uint16_t infoUpdateFrame;	//帧率
	bool     SendObject;      //发送对象标志
	bool     offLineFlag;		  //设备离线标志
}JudgeMonitor_t;

extern uint8_t         Judge_DataBuff[Judge_BuffSIZE];
extern uint16_t        JudgeDMALenth;
extern JudgeMonitor_t  JudgeMonitor;

/*比赛机器人状态*/
extern ext_game_robot_state_t    ext_game_robot_state;
/*空中机器人能量状态*/
extern aerial_robot_energy_t     aerial_robot_energy;
/*机器人增益*/
extern ext_buff_musk_t           ext_buff_musk;
/*实时射击信息*/
extern ext_shoot_data_t          ext_shoot_data;
/*客户端自定义数据*/
extern client_custom_data_t      client_custom_data;
/*机器人间通信*/
extern robot_interactive_data_t  RobotInteraction_Data;
/*机器人ID*/
extern robot_id_t robot_id;
/*客户端ID*/
extern client_id_t client_id;

uint8_t Get_CRC8_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint8_t ucCRC8);
uint32_t Verify_CRC8_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC8_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage,uint32_t dwLength,uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t * pchMessage,uint32_t dwLength);

void JudgeSystem_Init(void);
bool JudgeSystemTest(void);
void JudgeInfo_Process(uint8_t *JudgeInfo, uint16_t length);
void JudgeClientCustom_SendData(client_custom_data_t showdata, \
	                              uint8_t              speaker_id, \
                                uint16_t             client_id);
void JudgeRobotInteraction_SendData(robot_interactive_data_t speakdata, \
	                              uint8_t              speaker_id, \
                                uint16_t             listener_id);
float Get_realShotSpeed(void);
float Get_attackTime(void);
float Get_shooterOutput(void);
void setClient_pilotLamp(uint8_t LampNum, uint8_t status);
void setClient_showData(float data, uint8_t mark);
/******************************************************************************/


#endif /* __JUDGESYSTEM_H */



