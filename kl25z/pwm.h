#include "MKL25Z4.h"  // Include header for MKL25Z4 microcontroller definitions

#define MASK(x) (1 << (x))  // Macro to shift bits for pin

// Pin numbers for PWM output on Port C and Port D (for TPM0 - Motors 0, 1, 2)
#define PTC1_Pin 1  // Pin for PWM output (TPM0_CH0 - Motor 0 Forward)
#define PTC2_Pin 2  // Pin for PWM output (TPM0_CH1 - Motor 0 Reverse)
#define PTC3_Pin 3  // Pin for PWM output (TPM0_CH2 - Motor 1 Forward)
#define PTC4_Pin 4  // Pin for PWM output (TPM0_CH3 - Motor 1 Reverse)
#define PTD4_Pin 4  // Pin for PWM output (TPM0_CH4 - Motor 2 Forward)
#define PTD5_Pin 5  // Pin for PWM output (TPM0_CH5 - Motor 2 Reverse)
// Pin numbers for PWM output on Port A (for TPM2 - Motor 3)
#define PTA1_Pin 1  // Pin for PWM output (TPM2_CH0 - Motor 3 Forward)
#define PTA2_Pin 2  // Pin for PWM output (TPM2_CH1 - Motor 3 Reverse)

//pin numbers for PWM on port B (TPM1 - buzzer)
#define PTB0_Pin 0  // Pin for PWM output (TPM1_CH0)

// defind musical note frequency in hertz
#define C4 262
#define D4 294
#define E4 330
#define F4 349
#define G4 392
#define A4 440
#define B4 494

void initPWM(void);
void setMotorSpeed(int motor, int speedForward, int speedReverse);
void moveForward(void);
void moveBackward(void);
void moveCW(void);
void moveACW(void);
void moveStop(void);

void delay(int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < 7000; j++) {
        }
    }
}

void initPWM(void) {
    // Enable clock for Port C, Port D, and Port A
    SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK | SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTA_MASK;

    // Set Pin Control Register (PCR) for PTC1 to PTC4 (TPM0 CH0 to CH3 - Motors 0 and 1)
    PORTC->PCR[PTC1_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC1_Pin] |= PORT_PCR_MUX(4);     // Set PTC1 to use TPM0 CH0 (Alt function 4)

    PORTC->PCR[PTC2_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC2_Pin] |= PORT_PCR_MUX(4);     // Set PTC2 to use TPM0 CH1 (Alt function 4)

    PORTC->PCR[PTC3_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC3_Pin] |= PORT_PCR_MUX(4);     // Set PTC3 to use TPM0 CH2 (Alt function 4)

    PORTC->PCR[PTC4_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTC->PCR[PTC4_Pin] |= PORT_PCR_MUX(4);     // Set PTC4 to use TPM0 CH3 (Alt function 4)

    // Set Pin Control Register (PCR) for PTD4, PTD5 (TPM0 CH4, CH5 - Motor 2)
    PORTD->PCR[PTD4_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[PTD4_Pin] |= PORT_PCR_MUX(4);     // Set PTD4 to use TPM0 CH4 (Alt function 4)

    PORTD->PCR[PTD5_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTD->PCR[PTD5_Pin] |= PORT_PCR_MUX(4);     // Set PTD5 to use TPM0 CH5 (Alt function 4)

    // Set Pin Control Register (PCR) for PTA1 and PTA2 (TPM2 CH0, CH1 - Motor 3)
    PORTA->PCR[PTA1_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[PTA1_Pin] |= PORT_PCR_MUX(3);    // Set PTA1 to use TPM2 CH0 (Alt function 3)

    PORTA->PCR[PTA2_Pin] &= ~PORT_PCR_MUX_MASK;
    PORTA->PCR[PTA2_Pin] |= PORT_PCR_MUX(3);    // Set PTA2 to use TPM2 CH1 (Alt function 3)

    // Enable clock for TPM0 (Motors 0, 1, 2) and TPM2 (Motor 3)
    SIM->SCGC6 |= SIM_SCGC6_TPM0_MASK | SIM_SCGC6_TPM2_MASK;

    // Select TPM clock source (48 MHz internal clock)
    SIM->SOPT2 &= ~SIM_SOPT2_TPMSRC_MASK;   // Clear TPM clock source bits
    SIM->SOPT2 |= SIM_SOPT2_TPMSRC(1);      // Set clock source to MCGFLLCLK (48 MHz)

    // Set PWM frequency useing 1kHz
    TPM0->MOD = (48000000 / 128) / 1000;  // 48 MHz clock, 128 prescaler, 1 kHz PWM (MOTOR 0-2)
    TPM2->MOD = (48000000 / 128) / 1000;  // Set the same frequency for TPM2 (Motor 3)

    // Enable TPM0 and TPM2 with prescaler 128 and clock mode (CMOD = 01)
    TPM0->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear previous settings
    TPM0->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set CMOD to up-count and PS to 128

    TPM2->SC &= ~((TPM_SC_CMOD_MASK) | (TPM_SC_PS_MASK));  // Clear previous settings
    TPM2->SC |= (TPM_SC_CMOD(1) | TPM_SC_PS(7));           // Set CMOD to up-count and PS to 128

    // Use edge-aligned PWM mode (up-counting mode)
    TPM0->SC &= ~(TPM_SC_CPWMS_MASK);
    TPM2->SC &= ~(TPM_SC_CPWMS_MASK);

    // Enable PWM on TPM0 Channels 0-5 and TPM2 Channels 0-1
    TPM0_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH0 //M0
    TPM0_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH1 //M0
    TPM0_C2SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH2 //M1
    TPM0_C3SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH3 //M1
    TPM0_C4SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH4 //M2
    TPM0_C5SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM0_CH5 //M2
    TPM2_C0SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM2_CH0 //M3
    TPM2_C1SC |= (TPM_CnSC_ELSB(1) | TPM_CnSC_MSB(1));  // Enable edge-aligned PWM on TPM2_CH1 //M4
}

void setMotorSpeed(int motor, int speedForward, int speedReverse) {
    int modValue = TPM0->MOD;  // Get the MOD value (determined by the PWM frequency)

    // Set duty cycle based on motor index
    switch (motor) {
        case 0:  // Motor 0 (TPM0_CH0 for forward, TPM0_CH1 for reverse)
            TPM0_C0V = (modValue * speedForward) / 100;  // Motor 1 Forward
            TPM0_C1V = (modValue * speedReverse) / 100;  // Motor 1 Reverse
            break;
        case 1:  // Motor 1 (TPM0_CH2 for forward, TPM0_CH3 for reverse)
            TPM0_C2V = (modValue * speedForward) / 100;  // Motor 2 Forward
            TPM0_C3V = (modValue * speedReverse) / 100;  // Motor 2 Reverse
            break;
        case 2:  // Motor 2 (TPM0_CH4 for forward, TPM0_CH5 for reverse)
            TPM0_C4V = (modValue * speedForward) / 100;  // Motor 3 Forward
            TPM0_C5V = (modValue * speedReverse) / 100;  // Motor 3 Reverse
            break;
        case 3:  // Motor 3 (TPM2_CH0 for forward, TPM2_CH1 for reverse)
            modValue = TPM2->MOD;  // Use TPM2 MOD value for Motor 4
            TPM2_C0V = (modValue * speedForward) / 100;  // Motor 4 Forward
            TPM2_C1V = (modValue * speedReverse) / 100;  // Motor 4 Reverse
            break;
        default:
            break;
    }
}

void moveForward() {
    setMotorSpeed(0, 50, 0);
    setMotorSpeed(1, 50, 0);
    setMotorSpeed(2, 50, 0);
    setMotorSpeed(3, 50, 0);
}
void moveBackward(){
    setMotorSpeed(0, 0, 50);
    setMotorSpeed(1, 0, 50);
    setMotorSpeed(2, 0, 50);
    setMotorSpeed(3, 0, 50);
}

void moveCW() {
    setMotorSpeed(0, 0, 50);  // Motor 0 (Right Front) Reverse
    setMotorSpeed(1, 0, 50);  // Motor 1 (Right Back) Reverse
    setMotorSpeed(2, 50, 0);  // Motor 2 (Left Front) Forward
    setMotorSpeed(3, 50, 0);  // Motor 3 (Left Back) Forward
}

void moveACW(){
    setMotorSpeed(0, 50, 0);  
    setMotorSpeed(1, 50, 0);  
    setMotorSpeed(2, 0, 50); 
    setMotorSpeed(3, 0, 50);  
}
void moveStop(){
    setMotorSpeed(0, 0, 0);
    setMotorSpeed(1, 0, 0);
    setMotorSpeed(2, 0, 0);
    setMotorSpeed(3, 0, 0);
}



