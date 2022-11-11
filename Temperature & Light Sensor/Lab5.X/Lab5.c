#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

#define D1_RED      PORTBbits.RB3                               //Defines the ports that control LED1
#define D1_GREEN    PORTBbits.RB4
#define D1_BLUE     PORTBbits.RB5

#define D2_RED      PORTBbits.RB0                               //Defines the ports that control LED2
#define D2_GREEN    PORTBbits.RB1
#define D2_BLUE     PORTBbits.RB2

#define D3_RED      PORTAbits.RA4                               //Defines the ports that control LED3
#define D3_BLUE     PORTAbits.RA5

                                                                 //Prototypes
void Init_ADC(void);
void Init_TRIS(void);
unsigned int get_full_ADC(void);
void Select_ADC_Channel ( char);
void Delay_One_Sec(void);
void Init_UART(void);
void SET_D1_OFF(void);
void SET_D1_RED(void);
void SET_D1_GREEN(void);
void SET_D1_BLUE(void);
void SET_D1_WHITE(void);
void SET_D2_OFF(void);
void SET_D2_WHITE(void);
void SET_D3_RED(void);
void SET_D3_BLUE(void);
void SET_D3_PURPLE(void);

void Init_UART(void)                                            //Used for tera term
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


void main(void)
{
    Init_UART();                                                //Calls Init_UART Initializes tera term
    Init_ADC();                                                 //Calls Init_ADC() and Initializes ADCON ports
    Init_TRIS();                                                //Calls Init_TRIS() and Initializes TRISA-E as inputs or outputs
    char LED_7seg[10] = {0x01,0x4F, 0x12, 0x06, 
    0x4C, 0x24, 0x20, 0x0F, 0x00, 0x04};                        //array used to translate decimal -> 7segment
    
    while(1)
    {
        Select_ADC_Channel(0);                                  //Selects AN0
        int num_step = get_full_ADC();                          //Calls get_full_ADC() and returns the number of steps)
        int voltage_t = num_step*4;                             //Uses the number of steps to calculate voltage at AN0
        float tempC = (1035 - voltage_t) / 5.5;                 //Uses voltage to calculate temperature in C
        float tempF = (1.5 * tempC) + 32;                       //Converts C to F
        int tempF_int = (int) tempF;                            //Converts tempF from float into an int type
        char u = tempF_int / 10;                                //Divide tempF by 10 to get the upper digit
        char l = tempF_int % 10;                                //Modulo tempF by 10 to get the lower digit
        PORTD = LED_7seg[l];                                    //outputs the lower digit to PORTD
        char t = LED_7seg[u];                                   //store the 7segment number of u into t
        PORTC = t & 0x3f;                                       //mask in the lower 6 bits of t and output to PORTC
        PORTE = 0;                                              //PORTE is set to 0 and enables A of the seven segment
        if (t & 0x40) PORTE = 2;                                //If the 7th bit of t is 1, PORTE will output a 1, disabling A of the seven segment
        printf("temperature =%d\r\n", tempF_int);               //prints the variable tempF_int
        
         if (u ==0 )     SET_D2_OFF();                          //if the upper digit = 0 then LED2 will be off
         else if((u >= 1) & (u < 7))    PORTB = u;              //if 1<=u<7 then LED2 will display the color of the corresponding number of u
         else if (u>=7) SET_D2_WHITE();                         //if u>= 7, LED2 will be white
        
        if(tempF_int < 45)
        {
            SET_D1_OFF();                                       //If tempf_int is less than 45 D1 will be off
        }
        else if(tempF_int<=55)
        {
            SET_D1_RED();                                       //if tempF_int is <= 55 D1 is red
        }
        else if(tempF_int<=65 & tempF>55)
        {
            SET_D1_GREEN();                                     //if tempF_int is between 65 and 55, D1 is green
        }
        else if(tempF_int<=75 & tempF>65)
        {
            SET_D1_BLUE();                                      //if tempF_int is between 75 and 65, D1 is blue
        }
        else if(tempF_int>76)
        {
            SET_D1_WHITE();                                     //if tempF_int is greater than 76, D1 is white
        }
      
        Select_ADC_Channel(2);                                  //Selects AN2 as input
        int num_step = get_full_ADC();                          //calls get_full_ADC() to get number of steps
        float voltage_mv2 = num_step*4;                         //uses number of steps to calculate (mV) voltage at AN2
        printf("voltage =%f\r\n", voltage_mv2);                 //prints the  voltage in mV
       
        if (voltage_mv2 <2100)
        {
            SET_D3_RED();                                       // if mV is less than 2100, D3 is red
        }
        else if (voltage_mv2 >= 2100 & voltage_mv2 < 3000)
        {
            SET_D3_BLUE();                                      //if mV is between 2100 and 3000, D3 is Blue
        }
        else if(voltage_mv2 >= 3000)
        {
            SET_D3_PURPLE();                                    //if mV is bigger than 3000, D3 is purple
        }
        
       Delay_One_Sec();                                         //calls delay function to delay by 1 sec
    }
   
}

void Delay_One_Sec(void)
{
 for(int I=0; I <17000; I++);                                   // Delay function used to add a 1 second delay
 }

void Init_TRIS(void)
{
    TRISA = 0x0F;                                               //sets first 4 ports of A as inputs
    TRISB = 0x00;                                               //PORTB is all outputs
    TRISC = 0x00;                                               //PORTC is all outputs
    TRISD = 0x00;                                               //PORTD is all outputs
    TRISE = 0x00;                                               //PORTE is all outputs
}

void Select_ADC_Channel ( char channel )
{
    ADCON0 = channel *4 + 1;                                    //takes the chosen decimal channel and converts it to the binary number ADCON0 needs
}

void Init_ADC(void)
{
ADCON0=0x01;                                                    // select channel AN0, and turn on the ADC subsystem
 ADCON1=0x1B ;                                                  // select pins AN0 through AN3 as analog signal, VDD-VSS as
                                                                // reference voltage
 ADCON2=0xA9;                                                   // right justify the result. Set the bit conversion time (TAD) and
                                                                // acquisition time
}

unsigned int get_full_ADC(void)
{
    int result;
    ADCON0bits.GO=1;                                            // Start Conversion
    while(ADCON0bits.DONE==1);                                  // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                         // combine result of upper byte and
                                                                // lower byte into result
    return result;                                              // return the result.
}

void SET_D1_OFF()
{
    D1_RED = 0;                                                 //function that sets PORTB3-5 as 000, D1 is off, 
    D1_GREEN = 0;
    D1_BLUE = 0;
}

void SET_D1_RED()
{
    D1_RED = 1;                                                 //function that sets PORTB3-5 as 100, D1 is red
    D1_GREEN = 0;
    D1_BLUE = 0;
}

void SET_D1_GREEN()
{
    D1_RED = 0;                                                 //function that sets PORTB3-5 as 010,D1 green
    D1_GREEN = 1;
    D1_BLUE = 0;
}

void SET_D1_BLUE()
{
D1_RED = 0;                                                     //function that sets PORTB3-5 as 001, D1 blue
D1_GREEN = 0;
D1_BLUE = 1;
}

void SET_D1_WHITE()
{
D1_RED = 1;                                                     //function that sets PORTB3-5 as 111, D1 white
D1_GREEN = 1;
D1_BLUE = 1;
}

void SET_D2_OFF()
{
D2_RED = 0;                                                     //function that sets PORTB0-2 as 000, D2 off
D2_GREEN = 0;
D2_BLUE = 0;
}

void SET_D2_WHITE()
{
D2_RED = 1;                                                     //function that sets PORTB0-2 as 111, D2 white
D2_GREEN = 1;
D2_BLUE = 1;
}

void SET_D3_RED()
{
D3_RED = 1;                                                     //function that sets PORTA4-5 as 10, D3 red
D3_BLUE = 0;
}

void SET_D3_BLUE()
{
D3_RED = 0;                                                     //function that sets PORTA4-5 as 01, D3 blue
D3_BLUE = 1;
}

void SET_D3_PURPLE()
{
D3_RED = 1;                                                     //function that sets PORTA4-5 as 11, D3 purple
D3_BLUE = 1;
}