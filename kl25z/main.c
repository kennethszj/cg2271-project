#include "MKL25Z4.h"  // Include header for MKL25Z4 microcontroller definitions
#include "pwm.h"
#include "uart.h"

extern volatile uint8_t movement;


int main(void) {
    SystemCoreClockUpdate();  // Update system clock to reflect current speed
    initPWM();  // Initialize PWM for motors
		Init_UART2();
		while (1) {
			if (movement == MOVE_STOP) {
				moveStop();
			} else if (movement == MOVE_FORWARD) {
				moveForward();
			} else if (movement == MOVE_BACK) {
				moveBackward();
			} else if (movement == MOVE_ACW) {
				moveACW();
			} else if (movement == MOVE_CW) {
				moveCW();
			}
		}
}
