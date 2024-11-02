#include "MKL25Z4.h"  // Include header for MKL25Z4 microcontroller definitions
#include "pwm.h"
#include "uart.h"

extern volatile uint8_t global_move_state;


int main(void) {
    SystemCoreClockUpdate();  // Update system clock to reflect current speed
    initPWM();  // Initialize PWM for motors
		Init_UART2();
		while (1) {
			if (global_move_state == MOVE_STOP || global_move_state == COMPLETE) {
				moveStop();
			} else if (global_move_state == MOVE_FORWARD) {
				moveForward();
			} else if (global_move_state == MOVE_BACK) {
				moveBackward();
			} else if (global_move_state == MOVE_ACW) {
				moveACW();
			} else if (global_move_state == MOVE_CW) {
				moveCW();
			}
		}
}
