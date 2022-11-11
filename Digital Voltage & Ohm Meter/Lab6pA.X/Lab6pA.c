#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define DP    PORTDbits.RD7                                             //Defines PORTD pin 7

                                                                        //Prototypes
void Init_UART(void);
void putch(char);
void Select_ADC_Channel(char);
unsigned int get_full_ADC(void);
void Init_ADC(void);
void Delay_One_Sec(void);

void Init_UART(void)                                                    //function used to initialize tera term
{
 OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
USART_BRGH_HIGH, 25);
 OSCCON = 0x60;
}
void putch (char c)
{
 while (!TRMT);
 TXREG = c;
}

void main()
{
    Init_UART();                                                        //calls function that initializes tera term         
    Init_ADC();                                                         //calls function that initalizes  ADCON ports
    char LED_7seg[10] = {0x01,0x4F, 0x12, 0x06,                         //array used to translate decimal to 7segment
    0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};        
    
    TRISA = 0xFF;                                                       //TRISA is set as all inputs
    TRISC = 0x00;                                                       //TRISC is set as all outputs
    TRISD = 0x00;                                                       //TRISD is set as all outputs
    TRISE = 0x00;                                                       //TRISE is set as all outputs
    
    while(1)                                                            //while loop is used to keep reading AN0
    {
        Select_ADC_Channel(0);                                          //sets AN0 as an input
        float num_step = get_full_ADC();                                //calls get_full_ADC to calculate the number of steps
        float voltage = (num_step*4)/1000;                              //uses number of steps to convert to volts
        
        int u = (int)voltage;                                           //upper digit of the voltage is stored in u
        int l = (int)(voltage * 10)% 10;                                //lower digit of voltage is calculated and stored in l
        PORTD = LED_7seg[l];                                            //PORTD outputs the 7segment number that corresponds to l
        char t = LED_7seg[u];                                           //the 7segment number that u represents is stored in t
        PORTC = t & 0x3F;                                               //masks in lower 6 bits
        PORTE = 0;                                                      //PORTE outputs 0, A on 7seg turns on
        if(t & 0x40) PORTE=2;                                           //if the 7th bit of the 7seg that represents u is 1, the Pin1 of PORTE(controls A on 7seg) outputs 1, turns it off
        DP = 0;                                                         //outputs 0 to PORTD pin 7, turns it on
        printf("Voltage: %f\r\n", voltage);                             //prints the voltage
                  
   
        Delay_One_Sec();                                                //calls function that delays execution by 1 sec
    }
    
}

void Delay_One_Sec(void)
{
 for(int I=0; I <17000; I++);                                           // Delay function used to add a 1 second delay
 }


void Select_ADC_Channel ( char channel )
{
    ADCON0 = channel *4 + 1;                                            //calculation used to determine what ADCON port to take input from
}


unsigned int get_full_ADC(void)
{
    int result;
    ADCON0bits.GO=1;                                                    // Start Conversion
    while(ADCON0bits.DONE==1);                                          // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                 // combine result of upper byte and
                                                                        // lower byte into result
    return result;                                                      // return the result.
}

void Init_ADC(void)
{
    ADCON0 = 0x01;                                                      //selects AN0 as input, turns on ADC subsystem
    ADCON1 = 0x1E;                                                      //selects AN0-4 as analog signal, VDD-VSS as reference voltage
    ADCON2=0xA9;                                                        // right justify the result. Set the bit conversion time (TAD) and
                                                                        // acquisition time
}