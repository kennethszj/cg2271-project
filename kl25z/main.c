/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "uart.h"
#include "pwm.h"

volatile uint8_t global_move_state = 0;
volatile uint8_t global_started_state = 0;

void soundThread(void *arguments)
{
	for (;;)
	{
		playSong1();
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
	SystemCoreClockUpdate();  // Update system clock to reflect current speed
	initPWM();  // Initialize PWM for motors
	Init_UART2();

	osKernelInitialize();
	osThreadNew(soundThread, NULL, NULL);
	osThreadNew(motorThread, NULL, NULL);
	osThreadNew(frontLedThread, NULL, NULL);
	osThreadNew(rearLedThread, NULL, NULL);
	osKernelStart();
	for (;;) {}
}
