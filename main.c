#pragma config FOSC = INTRCCLK  // Oscillator Selection bits (INTOSC oscillator: CLKOUT function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-Up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // GP3/MCLR pin function select (GP3/MCLR pin function is MCLR)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

#define _XTAL_FREQ 4000000
#define PWM_Percent 12

#include <xc.h>

uint8_t nextState;

uint8_t CalcGCD(int a, int b) {
    int temp;
    while (a != 0) {
        temp = a;
        a = b % a; // remainder
        b = temp; // previous a becomes b
    }
    return b;
}

uint8_t CalcPWM_Ratio1(int Percentage) {
    return Percentage / CalcGCD(Percentage, 100);
}

uint8_t CalcPWM_Ratio2(int Percentage) {
    return (100 - Percentage) / CalcGCD(Percentage, 100);
}

void main(void) {
    uint8_t ratio1 = CalcPWM_Ratio1(PWM_Percent);
    uint8_t ratio2 = CalcPWM_Ratio2(PWM_Percent);
    OPTION_REG = 0b11000100;
    TRISIO2 = 0;
    TMR0 = 0;
    while (1) {
        if (TMR0 > ratio1 && nextState == 0) {
            GP2 = 1;
            nextState = 1;
            TMR0 = 0;
        }
        if (TMR0 > ratio2 && nextState == 1) {
            GP2 = 0;
            nextState = 0;
            TMR0 = 0;
        }
    }
}