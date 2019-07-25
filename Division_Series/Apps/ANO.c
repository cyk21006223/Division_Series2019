#include "ANO.h"

int8_t send_buf[15]= {0};

/* ACC_X, ACC_Y, ACC_Z, GYRO_X*/
void ANO_Send_Data_V1(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
	int8_t data_sum=0;
	int i=0;
	send_buf[0]=0xAA;
	send_buf[1]=0x01;
	send_buf[2]=0xAF;
	send_buf[3]=0x02; 
	send_buf[4]=0x08;
	send_buf[5]=Temp_Target1>>8;
	send_buf[6]=Temp_Target1;
	send_buf[7]=Temp_Now1>>8;
	send_buf[8]=Temp_Now1;
	send_buf[9]=Temp_Target2>>8;
	send_buf[10]=Temp_Target2;
	send_buf[11]=Temp_Now2>>8;
	send_buf[12]=Temp_Now2;
	for(i=0;i<13;i++)
	{
	data_sum+=send_buf[i];
	}
	send_buf[13]=data_sum;
	for(i=0;i<14;i++)
	{
	USART_sendChar(USART2,send_buf[i]);
	}

}
/*
//void ANO_Send_Data_V2(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2,int16_t Temp_Target3,int16_t Temp_Now3,int16_t Temp_Target4,int16_t Temp_Now4)
//{
//	int8_t data_sum=0;
//	int i=0,cout=0;
//	send_buf[cout++]=0xAA;
//	send_buf[cout++]=0x01;
//	send_buf[cout++]=0xAF;
//	send_buf[cout++]=0x02;
//	send_buf[cout++]=0;
//	send_buf[cout++]=Temp_Target1>>8;
//	send_buf[cout++]=Temp_Target1;
//	send_buf[cout++]=Temp_Now1>>8;
//	send_buf[cout++]=Temp_Now1;
//	send_buf[cout++]=Temp_Target2>>8;
//	send_buf[cout++]=Temp_Target2;
//	send_buf[cout++]=Temp_Now2>>8;
//	send_buf[cout++]=Temp_Now2;
//	send_buf[cout++]=Temp_Target3>>8;
//	send_buf[cout++]=Temp_Target3;
//	send_buf[cout++]=Temp_Now3>>8;
//	send_buf[cout++]=Temp_Now3;
//	send_buf[cout++]=Temp_Target4>>8;
//	send_buf[cout++]=Temp_Target4;
//	send_buf[cout++]=Temp_Now4>>8;
//	send_buf[cout++]=Temp_Now4;
//	send_buf[4]=cout-5;
//	for(i=0;i<cout;i++)
//	{
//	data_sum+=send_buf[i];
//	}
//	send_buf[cout++]=data_sum;
//	
//	for(i=0;i<cout;i++)
//	{
//	USART_sendChar(USART2,send_buf[i]);
//	}

//}
*/



void ANO_Send_Data_V2(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
	int8_t data_sum=0;
	int i=0,cout=0;
	send_buf[cout++]=0xAA;
	send_buf[cout++]=0x01;
	send_buf[cout++]=0xAF;
	send_buf[cout++]=0x02;
	send_buf[cout++]=0;
	send_buf[cout++]=Temp_Target1>>8;
	send_buf[cout++]=Temp_Target1;
	send_buf[cout++]=Temp_Now1>>8;
	send_buf[cout++]=Temp_Now1;
	send_buf[cout++]=Temp_Target2>>8;
	send_buf[cout++]=Temp_Target2;
	send_buf[cout++]=Temp_Now2>>8;
	send_buf[cout++]=Temp_Now2;
	send_buf[4]=cout-5;
	for(i=0;i<cout;i++)
	{
	data_sum+=send_buf[i];
	}
	send_buf[cout++]=data_sum;
	
	for(i=0;i<cout;i++)
	{
	USART_sendChar(USART2,send_buf[i]);
	}

}

void ANO_Send_Data_V3(int16_t Temp_Target1,int16_t Temp_Now1,int16_t Temp_Target2,int16_t Temp_Now2)
{
  int8_t data_sum=0;
	int i=0,cout=0;
	send_buf[cout++]=0xAA;
	send_buf[cout++]=0x01;
	send_buf[cout++]=0xAF;
	send_buf[cout++]=0xF1;
	send_buf[cout++]=0;
	send_buf[cout++]=Temp_Target1>>8;
	send_buf[cout++]=Temp_Target1;
	send_buf[cout++]=Temp_Now1>>8;
	send_buf[cout++]=Temp_Now1;
	send_buf[cout++]=Temp_Target2>>8;
	send_buf[cout++]=Temp_Target2;
	send_buf[cout++]=Temp_Now2>>8;
	send_buf[cout++]=Temp_Now2;
	send_buf[4]=cout-5;
	for(i=0;i<cout;i++)
	{
	data_sum+=send_buf[i];
	}
	send_buf[cout++]=data_sum;
	
	for(i=0;i<cout;i++)
	{
	USART_sendChar(USART2,send_buf[i]);
	}



}

