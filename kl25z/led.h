#include "MKL25Z4.h"
#include "uart.h"

extern volatile uint8_t movement = 0;

volatile uint8_t red_led_state = 0; // [0,1]
#define RED_LED_PIN 10 // PTC10

volatile uint8_t green_led_state = 0; // [0,10]
#define GREEN_LED_PIN_1 21 // PTE21
#define GREEN_LED_PIN_2 22 // PTE22
#define GREEN_LED_PIN_3 1 // PTB1
#define GREEN_LED_PIN_4 2 // PTB2
#define GREEN_LED_PIN_5 3 // PTB3
#define GREEN_LED_PIN_6 8 // PTB8
#define GREEN_LED_PIN_7 9 // PTB9
#define GREEN_LED_PIN_8 10 // PTB10
#define GREEN_LED_PIN_9 11 // PTB11
#define GREEN_LED_PIN_10 11 // PTC11
int GREEN_LED_ARR[] = {GREEN_LED_PIN_1, GREEN_LED_PIN_2, GREEN_LED_PIN_3, GREEN_LED_PIN_4, GREEN_LED_PIN_5, GREEN_LED_PIN_6, GREEN_LED_PIN_7, GREEN_LED_PIN_8, GREEN_LED_PIN_9, GREEN_LED_PIN_10};

void initLED(void) {
    // Enable clock to PIT module
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    // TODO: Enable clock for LED ports
    // SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK;

    // TODO: Set PCR to alt mode for GPIO for LED pins
    // PORTC->PCR[PTC1_Pin] &= ~PORT_PCR_MUX_MASK;
    // PORTC->PCR[PTC1_Pin] |= PORT_PCR_MUX(1);

    // TODO: Set PDOR to output for all LED pins
    // PTC->PDDR |= MASK(PTC1_Pin);



    // Enable the PIT timers and allow it to run in debug mode
    PIT->MCR = 0x00;  // MCR = 0 to enable PIT

    // Load the start value for 250 ms (4 Hz)
    // PIT is clocked by the system clock (48 MHz), so we calculate:
    // Load value = (SYSTEM_CLOCK / Frequency) - 1
    // 250 ms => 4 Hz, Load value = (48,000,000 / 4) - 1 = 11,999,999
    PIT->CHANNEL[0].LDVAL = SYSTEM_CLOCK / 4 - 1; // Load the timer start value

    // Enable the interrupt for PIT channel 0
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK; // Enable PIT interrupt

    // Start the PIT timer
    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK; // Start the timer

    // Enable PIT interrupt in the NVIC
    NVIC_EnableIRQ(PIT_IRQn);
}

// PIT Interrupt Handler, triggered every 250ms.
void PIT_IRQHandler(void) {
    // Clear the interrupt flag by writing 1 to TFLG
    PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;

    // toggle red_led every 250ms
    red_led_state = red_led_state == 0 ? 1 : 0;
    
    // toggle green_led to increment and cycle from 0-9
    green_led_state = green_led_state == 9 ? 0 : green_led_state + 1;
}

void handleRedLed(void) {
    if (red_led_state == 1) {
        // PTC->PDOR |= MASK(RED_LED_PIN);
    } else {
        // PTC->PDOR &= ~MASK(RED_LED_PIN);
    }
}

void handleGreenLed(int state) {
    if (state ==  MOVE_STOP) {
        // set all pins to high
        // PTC->PDOR &= ~(MASK(Green_Pin_1) | MASK(Green_Pin_2) | MASK(Green_Pin_3) | MASK(Green_Pin_4) | MASK(Green_Pin_5) | MASK(Green_Pin_6) | MASK(Green_Pin_7) | MASK(Green_Pin_8) | MASK(Green_Pin_9) | MASK(Green_Pin_10));
		// PTC->PDOR |= MASK(Green_LED_array[Green_LED_index]);
    } else {
        // clear all other green led pins, set only pin corresponding to green_led_state to 1.
    }
}