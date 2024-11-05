#include "MKL25Z4.h"
#include "macros.h"

#define MASK(x) (1 << (x)) 
#define SYSTEM_CLOCK 48000000
extern volatile uint8_t global_move_state; 

volatile uint8_t rear_led_state = 1; // [0,1]
#define REAR_LED_PIN 5 // PTC5

volatile uint8_t front_led_state = 0; // [0,10]
#define FRONT_LED_PIN_1 9 // PTC9
#define FRONT_LED_PIN_2 8 // PTC8
#define FRONT_LED_PIN_3 5 // PTA5
#define FRONT_LED_PIN_4 11 // PTC11
#define FRONT_LED_PIN_5 10 // PTC10
#define FRONT_LED_PIN_6 6 // PTC6
#define FRONT_LED_PIN_7 4 // PTA4
#define FRONT_LED_PIN_8 12 // PTA12
#define FRONT_LED_PIN_9 0 // PTC0
#define FRONT_LED_PIN_10 7 // PTC7

int FRONT_LED_ARR[] = {FRONT_LED_PIN_1, FRONT_LED_PIN_2, FRONT_LED_PIN_3, FRONT_LED_PIN_4, FRONT_LED_PIN_5, FRONT_LED_PIN_6, FRONT_LED_PIN_7, FRONT_LED_PIN_8, FRONT_LED_PIN_9, FRONT_LED_PIN_10};

void initLED(void) {
    // Enable clock to PIT module
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;

    // TODO: Enable clock for LED ports
    SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTA_MASK;

    // TODO: Set PCR to alt mode for GPIO for LED pins
    PORTC->PCR[FRONT_LED_PIN_1] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_1] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_2] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_2] |= PORT_PCR_MUX(1);
    PORTA->PCR[FRONT_LED_PIN_3] &= ~PORT_PCR_MUX_MASK; //PTA
    PORTA->PCR[FRONT_LED_PIN_3] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_4] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_4] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_5] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_5] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_6] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_6] |= PORT_PCR_MUX(1);
    PORTA->PCR[FRONT_LED_PIN_7] &= ~PORT_PCR_MUX_MASK; //PTA
    PORTA->PCR[FRONT_LED_PIN_7] |= PORT_PCR_MUX(1);
    PORTA->PCR[FRONT_LED_PIN_8] &= ~PORT_PCR_MUX_MASK; //PTA
    PORTA->PCR[FRONT_LED_PIN_8] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_9] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_9] |= PORT_PCR_MUX(1);
    PORTC->PCR[FRONT_LED_PIN_10] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[FRONT_LED_PIN_10] |= PORT_PCR_MUX(1);
    PORTC->PCR[REAR_LED_PIN] &= ~PORT_PCR_MUX_MASK; //PTC
    PORTC->PCR[REAR_LED_PIN] |= PORT_PCR_MUX(1);


    // TODO: Set PDOR to output for all LED pins
    PTC->PDDR |= MASK(FRONT_LED_PIN_1); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_2); 
    PTA->PDDR |= MASK(FRONT_LED_PIN_3); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_4); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_5); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_6); 
    PTA->PDDR |= MASK(FRONT_LED_PIN_7); 
    PTA->PDDR |= MASK(FRONT_LED_PIN_8); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_9); 
    PTC->PDDR |= MASK(FRONT_LED_PIN_10); 
    PTC->PDDR |= MASK(REAR_LED_PIN);



    // Enable the PIT timers and allow it to run in debug mode
    PIT->MCR = 0x00;  // MCR = 0 to enable PIT

    // Load the start value for 250 ms (4 Hz)
    // PIT is clocked by the system clock (48 MHz), so we calculate:
    // Load value = (SYSTEM_CLOCK / Frequency) - 1
    // 2 Hz, Load value = (48,000,000 / 16) - 1
    PIT->CHANNEL[0].LDVAL = SYSTEM_CLOCK / 16 - 1; // Load the timer start value

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

    // toggle rear led every 250ms
    rear_led_state = rear_led_state == 3 ? 0 : rear_led_state + 1;
    
    // toggle front led to increment and cycle from 0-9
    front_led_state = front_led_state == 9 ? 0 : front_led_state + 1;
}

void handleRearLed(void) { //when stop
		if (global_move_state == MOVE_STOP || global_move_state == COMPLETE) {
			if (rear_led_state % 2 == 0) {
					PTC->PDOR |= MASK(REAR_LED_PIN); //set to high
			} else {
					PTC->PDOR &= ~MASK(REAR_LED_PIN);
			}
		} else {
			if (rear_led_state == 1) {
					PTC->PDOR |= MASK(REAR_LED_PIN); //set to high
			} else if (rear_led_state == 3) {
					PTC->PDOR &= ~MASK(REAR_LED_PIN);
			}
		}
}

void handleFrontLed(void) {
    if (global_move_state ==  MOVE_STOP || global_move_state == COMPLETE) {
        // set all pins to high
        PTC->PDOR |= (MASK(FRONT_LED_PIN_1) | MASK(FRONT_LED_PIN_2) | MASK(FRONT_LED_PIN_4) | MASK(FRONT_LED_PIN_5) | MASK(FRONT_LED_PIN_6) |  MASK(FRONT_LED_PIN_9) | MASK(FRONT_LED_PIN_10));
				PTA->PDOR |= MASK(FRONT_LED_PIN_3) |MASK(FRONT_LED_PIN_7) | MASK(FRONT_LED_PIN_8);
    } else {
        // clear all other front led pins, set only pin corresponding to front_led_state to 1.
        for (int i = 0; i < 10; i++) {
            if (i == 2 || i == 6 || i == 7) {
                PTA->PDOR &= ~MASK(FRONT_LED_ARR[i]); // off led on port a
            } else {
                PTC->PDOR &= ~MASK(FRONT_LED_ARR[i]); //off led on ptc
            }
        }
        
        // set pin corresponding to front_led_state to 1
        int led_index = FRONT_LED_ARR[front_led_state];
        if (front_led_state == 2 || front_led_state == 6 || front_led_state == 7) {
            PTA->PDOR |= MASK(led_index); // on led on pta
        } else {
            PTC->PDOR |= MASK(led_index); // on led on ptc
        }
    }
}
