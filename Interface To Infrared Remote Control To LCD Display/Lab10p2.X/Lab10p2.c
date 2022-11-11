//Interface to Infrared Remote Control

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF
#pragma config CCP2MX = PORTBE
 
#define _XTAL_FREQ  8000000                                                     // Set operation for 8 Mhz
#include "ST7735.h"                                                             //includes the header file for the LCD

void TIMER1_isr(void);                                                          //protoypes
void INT0_isr(void);
void Initialize_Screen();
void Activate_Buzzer(void);
void Deactivate_Buzzer(void); 
void Wait_Half_Second(void);

unsigned char Nec_state = 0;                                                    //global variables
unsigned char i,bit_count;
short nec_ok = 0;
unsigned long long Nec_code;
char Nec_code1;
unsigned int Time_Elapsed;

                                                                                // color definitions for LCD screen
#define RD               ST7735_RED                             
#define CY               ST7735_CYAN
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_WHITE

#define Circle_Size     20                                                      // Size of Circle for Light
#define Circle_X        60                                                      // Location of Circle
#define Circle_Y        80                                                      // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1                                                       // Size of Normal Text
#define TS_2            2                                                       // Size of Big Text

char buffer[31];                                                                // general buffer for display purpose
char *nbr;                                                                      // general pointer used for buffer
char *txt;

char array1[21]={0xa2, 0x62, 0xe2, 0x22, 0x02, 0xc2, 0xe0, 0xa8, 0x90, 0x68,    //array to hold remote button codes
            0x98, 0xb0, 0x30, 0x18, 0x7a, 0x10, 0x38, 0x5a, 0x42, 0x4a, 0x52 };
char txt1[21][4] ={"CH-\0", "CH \0","CH+\0","|<<\0", ">>|\0", ">||\0", " - \0", //array to hold remote button names
            " + \0", "EQ \0", " 0 \0", "100\0", "200\0", " 1 \0", " 2 \0",
            " 3 \0", " 4 \0", " 5 \0", " 6 \0", " 7 \0", " 8 \0", " 9 \0"};
int color[21]={RD, RD, RD, CY, CY, GR, BU, BU, MA,                              //array to hold remote button colors
            BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK, BK};

char D1[21]= {0x01, 0x01, 0x01, 0x06,0x06, 0x02, 0x04,                          //array to hold LED1 colors
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
char D2[21] = {0,0,0,0,0,0,0, 0x04, 0x05, 0x07,                                 //array to hold LED2 colors
                0x07, 0x07, 0x07, 0x07, 0, 0, 0, 0, 0, 0, 0};
char D3[21] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                        //array to hold LED3 colors
                0x38, 0x38, 0x38, 0x38, 0x38, 0x38, 0x38};

void putch (char c)                                                             
{
    while (!TRMT);
    TXREG = c;
}
void init_UART()                                                                //function used to initialize tera term
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF &
    USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX &
    USART_BRGH_HIGH, 25);
    //OSCCON = 0x60;
}

void interrupt high_priority chkisr()
{
    if (PIR1bits.TMR1IF == 1) TIMER1_isr();                                     //checks if TIMER1 interrupt occurred
    if (INTCONbits.INT0IF == 1) INT0_isr();                                     //checks if INTO occurred
}

void TIMER1_isr(void)
{
    Nec_state = 0;                                                              // Reset decoding process
    INTCON2bits.INTEDG0 = 0;                                                    // Edge programming for INT0 falling edge
    T1CONbits.TMR1ON = 0;                                                       // Disable T1 Timer
    PIR1bits.TMR1IF = 0;                                                        // Clear interrupt flag
}

void force_nec_state0()                                                         //function that clears Nec_state and stops TMR1
{
    Nec_state=0;
    T1CONbits.TMR1ON = 0;
}

void INT0_isr(void)
{
    INTCONbits.INT0IF = 0;                                                      // Clear external interrupt
    if (Nec_state != 0)
    {
        Time_Elapsed = (TMR1H << 8) | TMR1L;                                    // Store Timer1 value
        TMR1H = 0;                                                              // Reset Timer1
        TMR1L = 0;
    }
    
    switch(Nec_state)                                                           //switches depending on Nec_state
    {
        case 0 :
        {
                                                                                // Clear Timer 1
            TMR1H = 0;                                                          // Reset Timer1
            TMR1L = 0;                                                          //
            PIR1bits.TMR1IF = 0;                                                //TMR1 reg is not overflowing, set to 0
            T1CON= 0x90;                                                        // Program Timer1 mode with count = 1usec using System clock running at 8Mhz
            T1CONbits.TMR1ON = 1;                                               // Enable Timer 1
            bit_count = 0;                                                      // Force bit count (bit_count) to 0
            Nec_code = 0;                                                       // Set Nec_code = 0
            Nec_state = 1;                                                      // Set Nec_State to state 1
            INTCON2bits.INTEDG0 = 1;                                            // Change Edge interrupt of INT0 to Low to High            
            return;
        }
        
        case 1 :
        {
            if(Time_Elapsed > 8500 && Time_Elapsed < 9500) Nec_state = 2;       //checks the leading pulse stage 
            else force_nec_state0();                                            //if its not in the leading pulse, state is reset
            INTCON2bits.INTEDG0 = 0;                                            //high to low
            return;
        }
        
        case 2 :                           
        {
            if(Time_Elapsed > 4000 && Time_Elapsed < 5000) Nec_state = 3;       //checks the space stage
            else force_nec_state0();                                            //if not, state is reset
            INTCON2bits.INTEDG0 = 1;                                            //low to high
            return;
        }
        
        case 3 :                            
        {
            if(Time_Elapsed >= 400 && Time_Elapsed <= 700) Nec_state = 4;       //checks for the 562 usec pulse burst
            else force_nec_state0();                                            //if not state is reset
            INTCON2bits.INTEDG0 = 0;                                            //high to  low      
            return;
        }
        
        case 4 :                            
        {
            if(Time_Elapsed > 400 && Time_Elapsed < 1800)                       //checks for either a logical 0 or logical 1
            {
                Nec_code = Nec_code << 1;
                if(Time_Elapsed > 1000)                                         //if the space is longer than 1000sec
                {                                                               //a logical 1 is read
                    Nec_code = Nec_code +1;
                }
                bit_count = bit_count + 1;                                      //counts the total bits read
                if (bit_count > 31)
                {
                    nec_ok = 1;
                    INTCONbits.INT0IE =0;                                       //onces all the bits are read, sets INT0 IE
                    Nec_state = 0;                                              //Nec_state is reset
                }
                Nec_state = 3;                                                  //if logical 0 is read , goes back to state 3
            }
            else force_nec_state0();                                            //if logical 0 or 1 is not read,goes back to state 0
            INTCON2bits.INTEDG0 = 1;                                            //low to high
            return;
        }
    }
}

void main()
{
    init_UART();                                                                //initializes tera term
    OSCCON = 0x70;                                                              // 8 Mhz
    nRBPU = 0;                                                                  // Enable PORTB internal pull up resistor
    TRISA = 0x00;                                                               //PORTA as output
    TRISB = 0x01;                                                               //Pin 1 of PORTB is an input
    TRISC = 0x00;                                                               // PORTC as output
    TRISD = 0x00;                                                               //PORTD as all output
    TRISE = 0x00;                                                               //PORTE as all output
    ADCON1 = 0x0F;                                                              //sets all ports as digital
    Initialize_Screen();                                                        //initializes LCD screen
    INTCONbits.INT0IF = 0;                                                      // Clear external interrupt
    INTCON2bits.INTEDG0 = 0;                                                    // Edge programming for INT0 falling edge H to L
    INTCONbits.INT0IE = 1;                                                      // Enable external interrupt
    TMR1H = 0;                                                                  // Reset Timer1
    TMR1L = 0;                                                                   //
    PIR1bits.TMR1IF = 0;                                                        // Clear timer 1 interrupt flag
    PIE1bits.TMR1IE = 1;                                                        // Enable Timer 1 interrupt
    INTCONbits.PEIE = 1;                                                        // Enable Peripheral interrupt
    INTCONbits.GIE = 1;                                                         // Enable global interrupts
    nec_ok = 0;                                                                 // Clear flag
    Nec_code = 0x0;                                                             // Clear code
    
    
 
    
    while(1)
    {
        if (nec_ok == 1)                                                        //if the whole NEC code is read
        {
            nec_ok = 0;                                                         //resets nec_ok

            Nec_code1 = (char) ((Nec_code >> 8));                               //reads the button code that was sent
            printf ("NEC_Code = %08lx %x\r\n", Nec_code, Nec_code1);            //prints the button code in tera term
            INTCONbits.INT0IE = 1;                                              // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                                            // Edge programming for INT0 falling edge
            
            char found = 0xff;                                                  //create a key to help display the colors on LED and LCD
            
            for (int i = 0; i < 21; i++)                                        //searches the array for the matching code that was received from the remote
            {
                if (array1[i] == Nec_code1) 
                {
                    found = i;                                                  //sets found as the matching code in the array
                    break;
                }
                
            }
            
            if (found != 0xff)                                                  //once found is set
            {
                
                PORTE = D2[found];                                              //displays the the color on the corresponding LED
                PORTD = D3[found] | D1[found];                  
                
                
                
                
                fillCircle(Circle_X, Circle_Y, Circle_Size, color[found]);      //displays a picture of the button pressed on the LCD
                drawCircle(Circle_X, Circle_Y, Circle_Size, ST7735_WHITE);  
                drawtext(Text_X, Text_Y, txt1[found], 
                                ST7735_WHITE, ST7735_BLACK,TS_1); 
                
                Activate_Buzzer();                                              //activates the buzzer when a button is pressed
                Wait_Half_Second();                                             //buzzer beeps for half a second
                Deactivate_Buzzer();
   
            
            }
        }
    }
}

void wait_one_sec()                                                             //function used to wait one second
{
    for (int k=0;k<17000;k++);
}
void Initialize_Screen()                                                        //function used to initialize LCD screen
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L Spring 21-S1");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

void Activate_Buzzer(void)                                                      //function used to activate buzzer
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Deactivate_Buzzer(void)                                                    //function used to deactivate the buzzer
{
CCP2CON = 0x0;
 PORTBbits.RB3 = 0;                                                             //outputs a 0 at B3
} 

void Wait_Half_Second()
{
    T0CON = 0x02;                                                               // Timer 0, 16-bit mode, prescaler 1:8
    TMR0H = 0x0B;                                                               // set the upper byte of TMR
    TMR0L = 0xDB;                                                               // set the lower byte of TMR
    INTCONbits.TMR0IF = 0;                                                      // clear the Timer 0 flag
    T0CONbits.TMR0ON = 1;                                                       // Turn on the Timer 0
    while (INTCONbits.TMR0IF == 0);                                             // wait for the Timer Flag to be 1 for done
    T0CONbits.TMR0ON = 0;                                                       // turn off the Timer 0
}