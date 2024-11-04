#include "MKL25Z4.h"
#include "macros.h"

#define UART_RX_PORTE23 23
#define BAUD_RATE 9600
#define UART2_INT_PRIO 128
#define BAUD_RATE 9600

#define MASK(x) (1 << (x))

volatile uint8_t rx_data;

extern volatile uint8_t global_move_state;
extern volatile uint8_t global_started_state;

void Init_UART2()
{
  uint32_t divisor, bus_clock;

  // Enable clock for UART2 and PORTE
  SIM->SCGC4 |= SIM_SCGC4_UART2_MASK;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

  // Set up pin control for PORTE23 to UART
  PORTE->PCR[UART_RX_PORTE23] &= ~PORT_PCR_MUX_MASK; // Clear UART_RX_PORTE23
  PORTE->PCR[UART_RX_PORTE23] |= PORT_PCR_MUX(4);    // Set UART_RX_PORTE23 to RX

  // Disable transmitter and receiver before configuration
  UART2->C2 &= ~(UART_C2_RE_MASK);

  // Set baud rate for UART2
  bus_clock = (DEFAULT_SYSTEM_CLOCK / 2);
  divisor = bus_clock / (BAUD_RATE * 16);  // divisor = bus_clock / (16 * desired_baud_rate)
  UART2->BDL = UART_BDL_SBR(divisor);      // sets the lower 8 bits of the baud rate divisor in the BDL register
  UART2->BDH = UART_BDH_SBR(divisor >> 8); // sets the higher 8 bits of divisor in the BDH register.

  
	UART2->C2 &= ~(UART_C2_RIE_MASK); // disables the UART2 receiver interrupt temporarily

  // 8N1 UART frame: start bits = 1; stop bits = 1; data bits = 8; parity bits = None;
  UART2->C1 = 0; // disable parity and set 8 bits per frame
  UART2->S2 = 0; // LSB transmission
  UART2->C3 = 0; // disable error detection, 8 bit per frame

  // Enable UART2 module to receive data
	UART2->C2 |= UART_C2_RE_MASK;
	// TODO: consider if priority needs to be set
  NVIC_ClearPendingIRQ(UART2_IRQn); //  clears any pending interrupts for UART2
  NVIC_EnableIRQ(UART2_IRQn);       //  enables the UART2 interrupt
	UART2->C2 |= UART_C2_RIE_MASK;
	
	// Enable RIE
}

void UART2_IRQHandler()
{
  NVIC_ClearPendingIRQ(UART2_IRQn); // Clear pending UART interrupts to prevent repeats

  // Check if the receive data register is full via RDRF flag
  if (UART2->S1 & UART_S1_RDRF_MASK)
  {
    rx_data = UART2->D;
		if (rx_data == 0x10) { // stop
			global_move_state = MOVE_STOP;
		} else if (rx_data == 0x11) { // forward
			global_move_state = MOVE_FORWARD;
      global_started_state = 1;
		} else if (rx_data == 0x12) { // backward
			global_move_state = MOVE_BACK;
      global_started_state = 1;
		} else if (rx_data == 0x13) { // anticlockwise
			global_move_state = MOVE_ACW;
      global_started_state = 1;
		} else if (rx_data == 0x14) { // clockwise
			global_move_state = MOVE_CW;
      global_started_state = 1;
		} else if (rx_data == 0x15) {
      global_move_state = MOVE_FORWARD_HALF;
      global_started_state = 1;
    } else if (rx_data == 0x16) {
      global_move_state = MOVE_BACK_HALF;
      global_started_state = 1;
    } else if (rx_data == 0x17) {
      global_move_state = CURVE_ACW;
      global_started_state = 1;
    } else if (rx_data == 0x18) {
      global_move_state = CURVE_CW;
      global_started_state = 1;
    } else if (rx_data == 0x20) {
      global_move_state = COMPLETE;
      global_started_state = 0;
    }
  }
}
