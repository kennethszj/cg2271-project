#include "MKL25Z4.h"  // Include header for MKL25Z4 microcontroller definitions

#define MASK(x) (1 << (x))  // Macro to shift bits for pin
#define CLEAR 0xffffffff     // Macro to clear interrupt flags

// Define pin numbers for PWM output on Port B (for TPM1)
#define PTB0_Pin 0  // Pin for PWM output (TPM1_CH0)
#define PTB1_Pin 1  // Pin for PWM output (TPM1_CH1)

// Define pin numbers for PWM output on Port C (for TPM0)
#define PTC1_Pin 1  // Pin for PWM output (TPM0_CH0)
#define PTC2_Pin 2  // Pin for PWM output (TPM0_CH1)
#define PTC3_Pin 3  // Pin for PWM output (TPM0_CH2)
#define PTC4_Pin 4  // Pin for PWM output (TPM0_CH3)

// Define musical note frequencies in Hertz (Hz)
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

int currentNote = 0;  // Current note index
int notes[] = {C4, D4, E4, F4, G4, A4, B4};  // Array of note frequencies

void initPWM(void);
void setPWMFrequency(int frequency);
void delay(int n);

void delay(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 7000; j++) {
            // Simple busy-wait loop for delay, approximating milliseconds
        }
    }
}

/**
 * Initialize PWM using TPM1 on PB0 and PB1
 */
void initPWM(void) {
    // Enable clock for Port B
    SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Set Pin Control Register (PCR) for PB0 and PB1 to use TPM1 functionality
    // Clear MUX (Multiplexing) bits for PB0, then set Alt mode 3 (TPM1 CH0)
    PORTB->PCR[PTB0_Pin] &= ~PORT_PCR_MUX_MASK;  // Clear previous MUX settings for PB0
    PORTB->PCR[PTB0_Pin] |= PORT_PCR_MUX(3);     // Set PB0 to use TPM1 (Alt function 3)
    
    // Clear MUX bits for PB1, then set Alt mode 3 (TPM1 CH1)
    PORTB->PCR[PTB1_Pin] &= ~PORT_PCR_MUX_MASK;  // Clear previous MUX settings for PB1
    PORTB->PCR[PTB1_Pin] |= PORT_PCR_MUX(3);     // Set PB1 to use TPM1 (Alt function 3)

    // Enable clock for TPM1 (Timer/PWM Module 1)
    SIM->SCGC6 |= SIM_SCGC6_TPM1_MASK;

    // Select TPM1 clock source (48 MHz internal clock)
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;   // Clear TPM clock source bits
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);      // Set clock source to MCGFLLCLK (48 MHz)

    // Set initial PWM frequency (C4 = 262 Hz)
    setPWMFrequency(C4);
}

/**
 * Set the PWM frequency by configuring the MOD register and duty cycle
 */
void setPWMFrequency(int frequency) {
    int clockFreq = 48000000;  // 48 MHz clock frequency
    int prescaler = 128;       // Prescaler divides the clock frequency
    int modValue;

    // Calculate the MOD value based on the desired frequency
    modValue = (clockFreq / prescaler) / frequency;

    // Set the MOD register to determine the PWM period
    TPM1->MOD = modValue;

    // Set duty cycle to 50% by setting Channel 0 Value (C0V)
    TPM1_C0V = modValue / 2;

    // Enable TPM1 with prescaler 128 and clock mode (CMOD = 01)
    TPM1->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear previous settings
    TPM1->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set CMOD to up-count and PS to 128

    // Use edge-aligned PWM mode (up-counting mode)
    TPM1->SC &= ~(TPM_SC_CPWMS_MASK);

    // Enable PWM on TPM1 Channel 0 (PB0)
    TPM1_C0SC &= ~((TPM_CnSC_ELSB_MASK) | (TPM_CnSC_ELSA_MASK) |
                   (TPM_CnSC_MSB_MASK) | (TPM_CnSC_MSA_MASK));  // Clear previous settings
    TPM1_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Set edge-aligned PWM (high-true pulses)
}

int main(void) {
    SystemCoreClockUpdate();  // Update system clock to reflect current speed
    initPWM();  // Initialize PWM on TPM1

    while (1) {
        setPWMFrequency(notes[currentNote]);  // Play current note

        currentNote++;  // Move to the next note
        if (currentNote >= 7) {
            currentNote = 0;  // Wrap around to the first note
        }

        delay(2000);  // Wait for 2 seconds before switching to the next note
    }
}


/**

The MKL25Z4 has multiple TPM modules (TPM0, TPM1, TPM2, etc.), and in your case, you're using TPM1, which has two channels (CH0 and CH1). In your code:

Channel 0 (CH0) is connected to PB0.
Channel 1 (CH1) is connected to PB1.

 */

