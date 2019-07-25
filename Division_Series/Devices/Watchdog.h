#ifndef __WATCHDOG_H
#define __WATCHDOG_H


#include "Users.h"



#define WATCHDOG_RESET_MS 	150	/*���Ź���λʱ��*/
#define watchdogReset() 	(IWDG_ReloadCounter())


void watchdogInit(u16 xms);
bool watchdogTest(void);


#endif 
