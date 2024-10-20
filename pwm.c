#include "MKL25Z4.h"

#define MASK(x) (1 << (x))  // Macro to shift bits for pin

//pin numbers for PWM output on Port C (for TPM0) (motor 0-3)
#define PTC1_Pin 1  //Pin for PWM output (TPM0_CH0 - Motor 0 forward)
#define PTC2_Pin 2  //Pin for PWM output (TPM0_CH1 - Motor 0 reverse)
#define PTC3_Pin 3  //Pin for PWM output (TPM0_CH2 - Motor 1 forward)
#define PTC4_Pin 4  //Pin for PWM output (TPM0_CH3 - Motor 1 reverse)
#define PTD4_Pin 4  //Pin for PWM output (TPM0_CH4 - Motor 2 forward)
#define PTD5_Pin 5  //Pin for PWM output (TPM0_CH5 - Motor 2 reverse)
//pin numbers for PWM output on Port B (for TPM2)  (motor 3)
#define PTA1_Pin 1  //Pin for PWM output (TPM2_CH0 - Motor 3 forward)
#define PTA2_Pin 2  //Pin for PWM output (TPM2_CH1 - Motor 3 reverse)

void initPWM(void);
void setMotorSpeed(int motor, int speedForward, int speedReverse);

void delay(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 7000; j++) { //delay in milisex
        }
    }
}

/**
 * Initialize PWM using TPM0 and TPM2 for motors
 */
void initPWM(void) {
    // Enable clock for Port C, Port D, and Port B
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTB_MASK;

    //Set PCRs for PTC1 to PTC4 (TPM0 CH0 to CH3 - Motors 0 and 1)
    PORTC->PCR[PTC1_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC1_Pin] |= PORT_PCR_MUX(4);     //Set PTC1 to use TPM0 CH0 (Alt function 4)

    PORTC->PCR[PTC2_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC2_Pin] |= PORT_PCR_MUX(4);     //Set PTC2 to use TPM0 CH1 (Alt function 4)

    PORTC->PCR[PTC3_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC3_Pin] |= PORT_PCR_MUX(4);     //Set PTC3 to use TPM0 CH2 (Alt function 4)

    PORTC->PCR[PTC4_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC4_Pin] |= PORT_PCR_MUX(4);     //Set PTC4 to use TPM0 CH3 (Alt function 4)

    //Set PCR for PTD4, PTD5 (TPM0 CH4, CH5 - Motor 2)
    PORTD->PCR[PTD4_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[PTD4_Pin] |= PORT_PCR_MUX(4);     //Set PTD4 to use TPM0 CH4 (Alt function 4)

    PORTD->PCR[PTD5_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[PTD5_Pin] |= PORT_PCR_MUX(4);     //Set PTD5 to use TPM0 CH5 (Alt function 4)

    //Set PCR for PTB18, PTB19 (TPM2 CH0, CH1 - Motor 3)
    PORTB->PCR[PTB18_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB18_Pin] |= PORT_PCR_MUX(3);    //Set PTB18 to use TPM2 CH0 (Alt function 3)

    PORTB->PCR[PTB19_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTB->PCR[PTB19_Pin] |= PORT_PCR_MUX(3);    //Set PTB19 to use TPM2 CH1 (Alt function 3)


    //Enable clock for TPM0 (Motor 0-2) and TPM2 (Motor 3)
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

    // Select TPM clock source (48 MHz internal clock)
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;   // Clear TPM clock source bits
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);      // Set clock source to MCGFLLCLK (48 MHz)

    // Set PWM frequency for motors (1 khz) (MOD = CLOCK_frequency / (prescaler x desired PWM frequency)  )
    TPM0->MOD = (48000000 / 128) / 1000;  // 48 MHz clock, 128 prescaler, 1 kHz PWM
    TPM2->MOD = (48000000 / 128) / 1000;  // Set the same frequency for TPM2 (Motor 4)

    // Enable TPM0 and TPM2 with prescaler 128 and clock mode (CMOD = 01)
    TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear previous settings
    TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set CMOD to up-count and PS to 128

    TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear previous settings
    TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set CMOD to up-count and PS to 128

    // Use edge-aligned PWM mode (up-counting mode)
    TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
    TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

    //Enable edge alighed PWM on TPM0 Channels 0-5 and TPM2 Channels 0-1 
    //Each motor need 2 PWM. One for forward 1 for reverse. So got 8)
    TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable PWM on TPM0_CH0
    TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM0_CH1
    TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM0_CH2
    TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM0_CH3
    TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM0_CH4
    TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM0_CH5
    TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM2_CH0
    TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  //Enable for TPM2_CH1
}

/**
 * Set the PWM duty cycle for a motor (forward and reverse)
 * motor: 0-3
 * speedForward: 0-100 (percentage of forward duty cycle)
 * speedReverse: 0-100 (percentage of reverse duty cycle)
 */
void setMotorSpeed(int motor, int speedForward, int speedReverse) {
    int modValue = TPM0->MOD;  //Get MOD value (determined by the PWM frequency)

    //Set duty cycle based on motor index (4 motor so 0-3)
    switch (motor) {
        case 0:  //Motor 1 (TPM0_CH0 for forward, TPM0_CH1 for reverse)
            TPM0_C0V = (modValue * speedForward) / 100;  // Motor 0 forward
            TPM0_C1V = (modValue * speedReverse) / 100;  // Motor 0 reverse

            break;
        case 1:  // Motor 2 (TPM0_CH2 for forward, TPM0_CH3 for reverse)
            TPM0_C2V = (modValue * speedForward) / 100;  // Motor 1 forward
            TPM0_C3V = (modValue * speedReverse) / 100;  // Motor 1 reverse

            break;
        case 2:  // Motor 3 (TPM0_CH4 for forward, TPM0_CH5 for reverse)
            TPM0_C4V = (modValue * speedForward) / 100;  // Motor 2 forward
            TPM0_C5V = (modValue * speedReverse) / 100;  // Motor 2 reverse

            break;
        case 3:  // Motor 3 (TPM2_CH0 for forward, TPM2_CH1 for reverse)
            modValue = TPM2->MOD;  // Use TPM2 MOD value for Motor 3
            TPM2_C0V = (modValue * speedForward) / 100;  // Motor 3 forward
            TPM2_C1V = (modValue * speedReverse) / 100;  // Motor 3 reverse

            break;
        default:
            break;
    }
}

int main(void) {
    SystemCoreClockUpdate();  // Update system clock to reflect current speed
    initPWM();  // Initialize PWM for motors

    while (1) {
        // Motor 1 forward at 60% speed, reverse at 0%
        setMotorSpeed(0, 60, 0);

        //Motor 2 reverse at 40% speed, forward at 0%
        setMotorSpeed(1, 0, 40);

        delay(2000); 

        // reverse

        setMotorSpeed(0, 0, 60);
        setMotorSpeed(1, 40, 0); 

        delay(2000);  //2 seconds delay
    }
}
