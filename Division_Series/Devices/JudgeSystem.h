#ifndef __JUDGESYSTEM_H
#define __JUDGESYSTEM_H

/******************************************************************************/
#include "Users.h"
/*****************************���ݲ����޸�*************************************/
#define Judge_BuffSIZE 	        300u			//����ϵͳ����������

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

/*RM2019����ϵͳ����Э�鸽¼V1.1,P4��2֡ͷ��ϸ����*/
#define JudgeFrameHeaderSOF     0xA5                              //����ϵͳ֡ͷ
#define JudgeFrameHeaderLength   5                            //����ϵͳ֡ͷ����
#define JudgeCmdIDLength         2                           //����ϵͳCmdID����
#define JudgeFrameTailLength     2                            //����ϵͳ֡β����
#define DataOffsetFromSOF       JudgeFrameHeaderLength \
                                +JudgeCmdIDLength            //Data��ƫ����7�ֽ�
																
#define TotalLengthExceptData    JudgeFrameHeaderLength \
                                +JudgeCmdIDLength \
																+JudgeFrameTailLength  //����ϵͳ��Data����9�ֽ�
																
#define InteractiveHeaderLength  6                 //����ϵͳ��������ͷ�ṹ6�ֽ�
#define RobotInteractBuffLength  1        //�����˼�ͨ����ʵ�������ݳ���,С��113

/*****************************���ݲ����޸�*************************************/

#define InteractiveDataID       0x0265        //�������Զ�������˽�����������ID
                                              //����ID:0x0200~0x02FF
/*****************************���ݲ����޸�*************************************/
//����״̬����
typedef __packed struct
{
uint8_t game_type : 4;
uint8_t game_progress : 4;
uint16_t stage_remain_time;
} ext_game_state_t;

//�����������
typedef __packed struct
{
  uint8_t winner;
} ext_game_result_t;


//�����˴������
typedef __packed struct
{
  uint16_t robot_legion;
} ext_game_robot_survivors_t;


//�����¼�����
typedef __packed struct
{
  uint32_t event_type;
} ext_event_data_t;

//����վ������ʶ
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_projectile_step;
uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//����վԤԼ�ӵ�
typedef __packed struct
{
uint8_t supply_projectile_id;
uint8_t supply_robot_id;
uint8_t supply_num;
} ext_supply_projectile_booking_t;

//����������״̬
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

//ʵʱ������������
typedef __packed struct
{
  uint16_t chassis_volt;
  uint16_t chassis_current;
	float chassis_power;
  uint16_t chassis_power_buffer;
  uint16_t shooter_heat0;//17mmǹ������
  uint16_t shooter_heat1;
} ext_power_heat_data_t;

//������λ��
typedef __packed struct
{
  float x;
  float y;
  float z;
  float yaw;
} ext_game_robot_pos_t;

//����������
typedef __packed struct
{
  uint8_t power_rune_buff;
}ext_buff_musk_t;

//���л���������״̬
typedef __packed struct
{
  uint8_t energy_point;
  uint8_t attack_time;
} aerial_robot_energy_t;

//�˺�����
typedef __packed struct
{
  uint8_t armor_id : 4;
  uint8_t hurt_type : 4;
} ext_robot_hurt_t;


//ʵʱ�����Ϣ
typedef __packed struct
{
  uint8_t bullet_type;
  uint8_t bullet_freq;
	float bullet_speed;
} ext_shoot_data_t;


//�������ݽ�����Ϣ
typedef __packed struct
{
uint16_t data_cmd_id;
uint16_t send_ID;
uint16_t receiver_ID;
}ext_student_interactive_header_data_t;


//�ͻ����Զ�������
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

//�����˼�ͨ��
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
/*****************************���ݲ����޸�*************************************/

//����ϵͳ֡��
typedef  struct{
	int32_t  BulletLaunched;  //�ѷ��䵯��
	uint16_t infoUpdateFrame;	//֡��
	bool     SendObject;      //���Ͷ����־
	bool     offLineFlag;		  //�豸���߱�־
}JudgeMonitor_t;

extern uint8_t         Judge_DataBuff[Judge_BuffSIZE];
extern uint16_t        JudgeDMALenth;
extern JudgeMonitor_t  JudgeMonitor;

/*����������״̬*/
extern ext_game_robot_state_t    ext_game_robot_state;
/*���л���������״̬*/
extern aerial_robot_energy_t     aerial_robot_energy;
/*����������*/
extern ext_buff_musk_t           ext_buff_musk;
/*ʵʱ�����Ϣ*/
extern ext_shoot_data_t          ext_shoot_data;
/*�ͻ����Զ�������*/
extern client_custom_data_t      client_custom_data;
/*�����˼�ͨ��*/
extern robot_interactive_data_t  RobotInteraction_Data;
/*������ID*/
extern robot_id_t robot_id;
/*�ͻ���ID*/
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



