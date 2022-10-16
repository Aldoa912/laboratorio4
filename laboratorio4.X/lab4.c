/*
 * File:   lab4.c
 * Author: aldoa
 *
 * Created on 16 de octubre de 2022, 10:05 AM
 */


#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>

#define _XTAL_FREQ 4000000

uint8_t ADC;
uint8_t alarma;
uint8_t dis1;
uint8_t dis2;
void setup(void);
void setupADC(void);
void valores (int valor);
uint8_t DISPLAY[16] = {
    0b00111111,  //0
    0b00000110,  //1
    0b01011011,  //2
    0b01001111,  //3
    0b01100110,  //4
    0b01101101,  //5
    0b01111101,  //6
    0b00000111,  //7
    0b01111111,  //8
    0b01101111,  //9
    0b01110111,  //A
    0b01111100,  //b
    0b00111001,  //C
    0b01011110,  //d
    0b01111001,  //E
    0b01110001,  //F
    
};


//******************************************************************************
// CÃ³digo Principal
//******************************************************************************
void main(void) {
    
    setup();
    setupADC();
    
    while(1){
//******************************************************************************
// ADC
//******************************************************************************
        ADCON0bits.GO = 1;  // enciendo la bandera
        while(ADCON0bits.GO == 1){
            ;
        }
        ADIF = 0;           // apago la bandera
        ADC = ADRESH;
        alarma = ADC;
        valores(ADC);
//******************************************************************************
// DISPLAYS
//******************************************************************************       
        PORTBbits.RB4 = 1;
        PORTBbits.RB5 = 0;
        PORTD = DISPLAY[dis2];
        __delay_ms(10);
        
        PORTBbits.RB4 = 0;
        PORTBbits.RB5 = 1;
        PORTD = DISPLAY[dis1];
        __delay_ms(10);
      
                
        
//******************************************************************************
// CONTADOR
//******************************************************************************
        
        if(PORTBbits.RB1 == 1){
            while(PORTBbits.RB1 == 1){
                ;
            }
            PORTC ++;
        }
        else if(PORTBbits.RB2 == 1){
            while(PORTBbits.RB2 == 1){
                ;
            }
            PORTC -- ;
           
        }
        else if(PORTBbits.RB3){
            if (alarma>PORTC){
                PORTBbits.RB0 = 1;
            }
            else {
                PORTBbits.RB0 = 0;
            }
        }
        else{
        PORTC = PORTC;
        
        }
        }
        return; 
}

//******************************************************************************
// Funcion para valores de los displays
//******************************************************************************
void valores(int valor){
    dis1 = (uint8_t)(valor/16);
    dis2 = (uint8_t)(valor%16);
}

//******************************************************************************
// FunciÃ³n para configurar GPIOs
//******************************************************************************
void setup(void){
    ANSELH = 0;
    ANSEL = 0;
    TRISC = 0;
    PORTC = 0;
    PORTB = 0;
    PORTD = 0;
    TRISD = 0;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 1;
    TRISBbits.TRISB3 = 1;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB0 = 0;
    OSCCONbits.IRCF = 0b110;        
    OSCCONbits.SCS = 1;             
    __delay_us(100);
}

void setupADC(void){
    
    // Paso 1 Seleccionar puerto de entrada
    //TRISAbits.TRISA0 = 1;
    TRISA = TRISA | 0x01;
    ANSEL = ANSEL | 0x01;
    
    // Paso 2 Configurar mÃ³dulo ADC
    
    ADCON0bits.ADCS1 = 0;
    ADCON0bits.ADCS0 = 1;       // Fosc/ 8
    
    ADCON1bits.VCFG1 = 0;       // Ref VSS
    ADCON1bits.VCFG0 = 0;       // Ref VDD
    
    ADCON1bits.ADFM = 0;        // Justificado hacia izquierda
    
    ADCON0bits.CHS3 = 0;
    ADCON0bits.CHS2 = 0;
    ADCON0bits.CHS1 = 0;
    ADCON0bits.CHS0 = 0;        // Canal AN0
    
    ADCON0bits.ADON = 1;        // Habilitamos el ADC
    __delay_us(100);
}