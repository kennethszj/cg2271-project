/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "pwm.h"
#include "led.h"

volatile uint8_t global_move_state = 0;
volatile uint8_t global_started_state = 0;

/*
Usage Example:
delay(0x80000);
*/


void soundThread(void *arguments)
{
	for (;;)
	{
		handleSound();
	}
}

void motorThread(void *arguments)
{
	for (;;)
	{
		handleMotor();
	}
}

void frontLedThread(void *arguments)
{
	for (;;)
	{
		handleFrontLed();
	}
}

void rearLedThread(void *arguments)
{
	for (;;)
	{
		handleRearLed();
	}
}

int main(void) {
	SystemCoreClockUpdate();
	initPWM();
	initUART2();
	initLED();

	osKernelInitialize();
	osThreadNew(soundThread, NULL, NULL);
	osThreadNew(frontLedThread, NULL, NULL);
	osThreadNew(rearLedThread, NULL, NULL);
	osThreadNew(motorThread, NULL, NULL);
	osKernelStart();
	for(;;){}
}
