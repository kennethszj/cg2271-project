#include "MKL25Z4.h"  // Include header for MKL25Z4 microcontroller definitions
#include "pwm.h"
#include "uart.h"

extern volatile uint8_t global_move_state;

void soundThread(void)
{
	for (;;)
	{
		handleSound();
	}
}

void motorThread(void)
{
	for (;;)
	{
		handleMotor();
	}
}

int main(void) {
    SystemCoreClockUpdate();  // Update system clock to reflect current speed
    initPWM();  // Initialize PWM for motors
	Init_UART2();

	osKernelInitialize();
	osKernelStart();
	osThreadNew(soundThread, NULL, NULL);
	osThreadNew(motorThread, NULL, NULL);
	for(;;) {}
}
