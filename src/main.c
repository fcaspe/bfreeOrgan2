/*
****************************************************************
*     _      __                                           ____  
*    | |__  / _|_ __ ___  ___  ___  _ __ __ _  __ _ _ __ |___ \ 
*    | '_ \| |_| '__/ _ \/ _ \/ _ \| '__/ _` |/ _` | '_ \  __) |
*    | |_) |  _| | |  __/  __/ (_) | | | (_| | (_| | | | |/ __/ 
*    |_.__/|_| |_|  \___|\___|\___/|_|  \__, |\__,_|_| |_|_____|
*                                       |___/   
*	  A flexynh based synchronous drawbar organ synthesizer.
****************************************************************	
	FILE: main.c
	VERSION: 0.1
	DATE: Jun 6th, 2019
	AUTHOR: Franco Caspe			

Copyright [2019] [Franco Caspe]
SPDX-License-Identifier: Apache-2.0

*/
 
#include "stm32f4xx.h"
#include "engine_discovery.hpp"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

int main(void)
{
	//Set clocks
	SystemInit();
	
	/* SETTING LEDS HERE ONLY FOR TESTBENCH */		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitTypeDef GPIO_InitDef;

	GPIO_InitDef.GPIO_Pin = GPIO_Pin_12| GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitDef.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitDef.GPIO_OType = GPIO_OType_PP;
	GPIO_InitDef.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitDef.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitDef);
	
	
	engine::engine_discovery engine;
	engine.init();
	engine.run();

	for(;;);
}
