#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE 

#define DP    PORTDbits.RD7                                         //Defines PORTD pin 7 as DP

                                                                    //Prototypes
void Init_UART(void);
void putch(char);
void Select_ADC_Channel(char);
unsigned int get_full_ADC(void);
void Init_ADC(void);
void Activate_Buzzer(void);
void Deactivate_Buzzer(void);
void Delay_One_Sec(void);

void Init_UART(void)                                                //function used to initialize tera term
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
    Init_UART();                                                    //calls function to initialize tera term
    Init_ADC();                                                     //calls function to initialize ADCON ports
    char LED_7seg[10] = {0x01,0x4F, 0x12, 0x06,                 
    0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};                            //array used to translate decimal to 7segment number
    
    TRISA = 0xFF;                                                   //sets PORTA as all inputs
    TRISB = 0x00;                                                   //sets PORTB as all outputs
    TRISC = 0x00;                                                   //sets PORTC as all outputs
    TRISD = 0x00;                                                   //sets PORTD as all outputs
    TRISE = 0x00;                                                   //sets PORTE as all outputs
    
    while(1)                                                        //while loop used to continue reading ADCON ports
    {
        Select_ADC_Channel(1);                                      //channel for 1k resistor
        int num_step = get_full_ADC();                              //calls get_full_ADC() to calculate number of steps
        float voltage = (num_step*4.0)/1000.0;                      //uses number of steps to calculate voltage
        float RL = (1.0 * voltage) / (4.096 - voltage);             //calculates RL by using the voltage
        
        if (RL < 10.0)                                              //if RL is less than 10, will enable the 7seg as X.X
        {
            int u = (int)RL;                                        //takes upper digit of RL by changing RL data type
            int l = (int)(RL * 10)% 10;                             //takes the lower 0.X digit of RL
            PORTD = LED_7seg[l];                                    //PORTD outputs the 7seg number that corresponds to l
            char t = LED_7seg[u];                                   //takes the 7seg number that corresponds to u and stores in t
            PORTC = t & 0x3F;                                       //masks in lower 6 bits
            PORTE = 0;                                              //PORTE outputs 0, turns the A on 7seg on
            if(t & 0x40) PORTE=2;                                   //if the 7 bit of t is 1, PORTE pin 1 outputs 1, turns A on 7seg off
            DP = 0;                                                 //Turns DP on
        printf("RL: %f\r\n", RL);                                   //Prints the calculated float value of RL
        } 
        else                                                        //if RL isn't less than 10, will run this instead
        {
            int RLint = (int)RL;                                    //changes RL to an int data type
            int u = RLint / 10;                                     //takes the upper digit of RLint
            int l = RLint % 10;                                     //takes the lower digit of RLint
            PORTD = LED_7seg[l];                                    //PORTD outputs the 7seg number that corresponds to l
            char t = LED_7seg[u];                                   //stores the 7seg number that corresponds to u in t
            PORTC = t & 0x3F;                                       //masks the lower 6 digits of t and output to PORTC
            PORTE = 0;                                              //PORTE outputs all 0, turning A on 7seg on
            if(t & 0x40) PORTE=2;                                   //if the 7th bit of t is 1, pin1 of PORTE outputs 1, turning A on 7seg off
            DP = 1;                                                 //DP is turned off
            printf("RL: %f\r\n", RL);                               //Prints calculated float value of RL
        }
        
        char temp = (int) RL/10;                                    //stores the upper digit of RL in temp
        if (temp >= 7) PORTB = 0x07;                                //if temp is larger or equal to 7, the pins0-2 of PORTB are set to 111, LED1 is white
        else PORTB = temp;                                          //any other values of temp, pins0-2 of PORTB are set to temp in binary, LED1 is set to temp's corresponding color
        
        if(RL <= 0.070)                                             //If RL is less than 0.07
        {
            PORTBbits.RB4 = 1;                                      //PORTB pins4-5 are set to 11, LED3 is yellow
            PORTBbits.RB5 = 1;
            Activate_Buzzer();                                      //calls function that activates buzzer
        } else                                                      //if RL is any other value
        {
            PORTBbits.RB4 = 0;                                      //PORTB pins4-5 are set to 00, LED3 is off
            PORTBbits.RB5 = 0;
            Deactivate_Buzzer();                                    //calls function that deactivates buzzer
        }
        
        Delay_One_Sec();                                            //calls function that delays execution by 1 sec
    }
    
}

void Delay_One_Sec(void)
{
 for(int I=0; I <17000; I++);                                       // Delay function used to add a 1 second delay
 }


void Select_ADC_Channel ( char channel )
{
    ADCON0 = channel *4 + 1;                                        //function used to calculate which AN port as input
}


unsigned int get_full_ADC(void)
{
    int result;
    ADCON0bits.GO=1;                                                // Start Conversion
    while(ADCON0bits.DONE==1);                                      // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                             // combine result of upper byte and
                                                                    // lower byte into result
    return result;                                                  // return the result.
}

void Init_ADC(void)
{
    ADCON0 = 0x01;                                                  //selects AN0 as input, turns on ADC subsystem
    ADCON1 = 0x1E;                                                  //selects AN0-4 as analog signal, VDD-VSS as reference voltage
    ADCON2=0xA9;                                                    // right justify the result. Set the bit conversion time (TAD) and
                                                                    // acquisition time
}

void Activate_Buzzer(void)                                          //function used to activate buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
    
}
void Deactivate_Buzzer(void)                                        //function used to deactivate buzzer
{
    CCP2CON = 0x0;
    PORTBbits.RB3 = 0;
} 