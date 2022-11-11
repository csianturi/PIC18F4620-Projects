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

#include "I2C_Soft.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "ST7735.h"

#define Circle_Size     20              // Size of Circle for Light
#define Circle_X        60              // Location of Circle
#define Circle_Y        80              // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of Big Text


// colors
#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_BLACK

#define kp               PORTEbits.RE1     

#define Circle_Size     20              // Size of Circle for Light
#define Circle_X        60              // Location of Circle
#define Circle_Y        80              // Location of Circle
#define Text_X          52
#define Text_Y          77
#define TS_1            1               // Size of Normal Text
#define TS_2            2               // Size of Big Text

#define RD               ST7735_RED
#define BU               ST7735_BLUE
#define CY               ST7735_CYAN
#define GR               ST7735_GREEN
#define MA               ST7735_MAGENTA
#define BK               ST7735_WHITE

short nec_ok = 0;

char Nec_code1;
char tempSecond = 0xff; 
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;

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


char TempSecond;

char buffer[31];                        // general buffer for display purpose
char *nbr;                              // general pointer used for buffer
char *txt;

char Nec_code1;
char found;

void Deactivate_Buzzer();
void Activate_Buzzer();
void Initialize_Screen();
void Wait_One_Sec();

void putch (char c)
{   
    while (!TRMT);       
    TXREG = c;
}

void init_UART()
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}


void Wait_One_Sec()
{
    for(int i=0; i <17000; i++);                                       // Delay function used to add a 1 second delay
}


void Do_Init()                      // Initialize the ports 
{ 
    init_UART();                    // Initialize the uart
    init_INTERRUPT();
    OSCCON=0x70;                    // Set oscillator to 8 MHz 
    
    ADCON1= 0x0F;		    // Fill out values
    TRISA = 0x14;                     //0001 0100
    TRISB = 0x01;
    TRISC = 0x00;                   
    TRISD = 0x00;
    TRISE = 0x00;
    RBPU=0;
    I2C_Init(100000); 
    
    Initialize_Screen();
    DS1621_Init();
    

}


void main() 
{ 
    Do_Init();                                                  // Initialization  
    
//    while (1)							// This is for the DS1621 testing. Comment this section
//    {								// out to move on to the next section
//        signed char tempC = DS1621_Read_Temp();
//        signed char tempF = (tempC * 9 / 5) + 32;
//        printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
//        Wait_One_Sec();
//    }
    
    
      while (1)						// This is for the DS3231 testing. Comment this section
      {							// out to move on to the next section

        DS3231_Read_Time();
        if(tempSecond != second)
        {
            tempSecond = second;
            signed char tempC = DS1621_Read_Temp();
            signed char tempF = (tempC * 9 / 5) + 32;
            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,minute,second,month,day,year);
            printf (" Temperature = %d degreesC = %d degreesF\r\n", tempC, tempF);
        }
    
        if (nec_ok == 1)                                                        //if the whole NEC code is read
        {
            
            nec_ok = 0;                                                         //resets nec_ok

            //Nec_code1 = (char) ((Nec_code >> 8));                               //reads the button code that was sent
            printf ("NEC_Code =  %x\r\n", Nec_code1);            //prints the button code in tera term
            INTCONbits.INT0IE = 1;                                              // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                                            // Edge programming for INT0 falling edge
            
            if(Nec_code1== 0x7a) DS3231_Setup_Time();
            
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
                Wait_One_Sec();                                             //buzzer beeps for half a second
                Deactivate_Buzzer();
   
            
            }
        }
      }
            
   
    
    
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

void Initialize_Screen()
{
    LCD_Reset();
    TFT_GreenTab_Initialize();
    fillScreen(ST7735_BLACK);
  
    /* TOP HEADER FIELD */
    txt = buffer;
    strcpy(txt, "ECE3301L Fall 22-S3");  
    drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    strcpy(txt, "LAB 10 ");  
    drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}




