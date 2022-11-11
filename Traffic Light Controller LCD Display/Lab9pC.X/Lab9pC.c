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

#define _XTAL_FREQ      8000000                                                 // Set operation for 8 Mhz
#define TMR_CLOCK       _XTAL_FREQ/4                                            // Timer Clock 2 Mhz

#define TFT_DC             	PORTCbits.RC0                                       // Location of TFT D/C
#define TFT_CS              PORTCbits.RC1                                       // Location of TFT Chip Select
#define TFT_RST            	PORTCbits.RC4                                       // Location of TFT Reset
#define SEC_LED             PORTDbits.RD7

#define NS_RED              PORTAbits.RA5                                       //definitions for LEDs and switches
#define NS_GREEN            PORTBbits.RB6

#define NSLT_RED            PORTEbits.RE1
#define NSLT_GREEN          PORTEbits.RE2

#define EW_RED              PORTAbits.RA3
#define EW_GREEN            PORTAbits.RA4

#define EWLT_RED            PORTAbits.RA1
#define EWLT_GREEN          PORTAbits.RA2

#define NS_LT_SW            PORTBbits.RB4

#define EW_LT_SW            PORTBbits.RB5

#define MODE_LED            PORTBbits.RB7

// colors
#define OFF 		0                                                           // Defines OFF as decimal value 0
#define RED 		1                                                           // Defines RED as decimal value 1
#define GREEN 		2                                                           // Defines GREEN as decimal value 2
#define YELLOW 		3                                                           // Defines YELLOW as decimal value 3

#define Circle_Size     7                                                       // Size of Circle for Light
#define Circle_Offset   15                                                      // Location of Circle
#define TS_1            1                                                       // Size of Normal Text
#define TS_2            2                                                       // Size of PED Text
#define Count_Offset    10                                                      // Location of Count

#define XTXT            30                                                      // X location of Title Text 
#define XRED            40                                                      // X location of Red Circle
#define XYEL            60                                                      // X location of Yellow Circle
#define XGRN            80                                                      // X location of Green Circle
#define XCNT            100                                                     // X location of Sec Count

#define EW              0                                                       // Number definition of East/West
#define EWLT            1                                                       // Number definition of East/West Left Turn
#define NS              2                                                       // Number definition of North/South
#define NSLT            3                                                       // Number definition of North/South Left Turn
 
#define Color_Off       0                                                       // Number definition of Off Color
#define Color_Red       1                                                       // Number definition of Red Color
#define Color_Green     2                                                       // Number definition of Green Color
#define Color_Yellow    3                                                       // Number definition of Yellow Color

#define EW_Txt_Y        20
#define EW_Cir_Y        EW_Txt_Y + Circle_Offset
#define EW_Count_Y      EW_Txt_Y + Count_Offset
#define EW_Color        ST7735_CYAN

#define EWLT_Txt_Y      50
#define EWLT_Cir_Y      EWLT_Txt_Y + Circle_Offset
#define EWLT_Count_Y    EWLT_Txt_Y + Count_Offset
#define EWLT_Color      ST7735_WHITE

#define NS_Txt_Y        80
#define NS_Cir_Y        NS_Txt_Y + Circle_Offset
#define NS_Count_Y      NS_Txt_Y + Count_Offset
#define NS_Color        ST7735_BLUE 

#define NSLT_Txt_Y      110
#define NSLT_Cir_Y      NSLT_Txt_Y + Circle_Offset
#define NSLT_Count_Y    NSLT_Txt_Y + Count_Offset
#define NSLT_Color      ST7735_MAGENTA

#define PED_EW_Count_Y  30
#define PED_NS_Count_Y  90
#define PED_Count_X     2
#define Switch_Txt_Y    140


#include "ST7735_TFT.c"                                                         //library used for TFT LCD

void init_UART(void);                                                           //prototypes
void putch(char);
void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
char buffer[31];                                                                // general buffer for display purpose
char *nbr;                                                                      // general pointer used for buffer
char *txt;

char EW_Count[]     = "00";                                                     // text storage for EW Count
char EWLT_Count[]   = "00";                                                     // text storage for EW Left Turn Count
char NS_Count[]     = "00";                                                     // text storage for NS Count
char NSLT_Count[]   = "00";                                                     // text storage for NS Left Turn Count

char PED_EW_Count[] = "00";                                                     // text storage for EW Pedestrian Count
char PED_NS_Count[] = "00";                                                     // text storage for NS Pedestrian Count

char SW_EWPED_Txt[] = "0";                                                      // text storage for EW Pedestrian Switch
char SW_EWLT_Txt[]  = "0";                                                      // text storage for EW Left Turn Switch
char SW_NSPED_Txt[] = "0";                                                      // text storage for NS Pedestrian Switch
char SW_NSLT_Txt[]  = "0";                                                      // text storage for NS Left Turn Switch

char SW_MODE_Txt[]  = "D";                                                      // text storage for Mode Light Sensor
	
char Act_Mode_Txt[]  = "D";                                                     // text storage for Actual Mode
char FlashingS_Txt[] = "0";                                                     // text storage for Emergency Status
char FlashingR_Txt[] = "0";                                                     // text storage for Flashing Request

char dir;
char Count;                                                                     // RAM variable for Second Count
char PED_Count;                                                                 // RAM variable for Second Pedestrian Count

char SW_EWPED;                                                                  // RAM variable for EW Pedestrian Switch
char SW_EWLT;                                                                   // RAM variable for EW Left Turn Switch
char SW_NSPED;                                                                  // RAM variable for NS Pedestrian Switch
char SW_NSLT;                                                                   // RAM variable for NS Left Turn Switch

char NS_PED_SW;                                                                 //RAM var for NSPEDSW
char EW_PED_SW;                                                                 //RAM var for EWPEDSW

char SW_MODE;                                                                   // RAM variable for Mode Light Sensor
int MODE;                                                                       
char direction;
float volt;
char FLASHING_REQUEST;
char FLASHING;

unsigned int get_full_ADC(void);                                                //prototypes
void Init_ADC(void);
void init_IO(void);


void Set_NS(char color);
void Set_NS_LT(char color);
void Set_EW(char color);
void Set_EW_LT(char color);

void PED_Control( char Direction, char Num_Sec);
void Day_Mode(void);
void Night_Mode(void);


void Wait_One_Second(void);
void Wait_Half_Second(void);
void Wait_N_Seconds (char);
void Wait_One_Second_With_Beep(void);

void update_LCD_color(char , char );
void update_LCD_PED_Count(char direction, char count);
void Initialize_Screen(void);
void update_LCD_misc();
void update_LCD_count(char, char);
void Do_Flashing();

char LED_7seg[10] = {0x40,0x79, 0x24, 0x30, 
    0x19, 0x12, 0x02, 0x78, 0x00, 0x18};                                        //array to hold decimal->7seg

void init_UART()                                                                //function used to initialize tera term
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

    


void Initialize_Screen()                                                        //function used to initialize LCD
{
  LCD_Reset();
  TFT_GreenTab_Initialize();
  fillScreen(ST7735_BLACK);
  
  /* TOP HEADER FIELD */
  txt = buffer;
  strcpy(txt, "ECE3301L Fall 22-S3");  
  drawtext(2, 2, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
  /* MODE FIELD */
  strcpy(txt, "Mode:");
  drawtext(2, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(35,10, Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);

  /* EMERGENCY REQUEST FIELD */
  strcpy(txt, "FR:");
  drawtext(50, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(70, 10, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
  /* EMERGENCY STATUS FIELD */
  strcpy(txt, "FS:");
  drawtext(80, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(100, 10, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1); 
  
  /* SECOND UPDATE FIELD */
  strcpy(txt, "*");
  drawtext(120, 10, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
      
  /* EAST/WEST UPDATE FIELD */
  strcpy(txt, "EAST/WEST");
  drawtext  (XTXT, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, EW_Cir_Y-8, 60, 18, EW_Color);
  fillCircle(XRED, EW_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, EW_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, EW_Cir_Y, Circle_Size, ST7735_GREEN);  
  drawtext  (XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);

  /* EAST/WEST LEFT TURN UPDATE FIELD */
  strcpy(txt, "E/W LT");
  drawtext  (XTXT, EWLT_Txt_Y, txt, EWLT_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, EWLT_Cir_Y-8, 60, 18, EWLT_Color);  
  fillCircle(XRED, EWLT_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, EWLT_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, EWLT_Cir_Y, Circle_Size, ST7735_GREEN);   
  drawtext  (XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);

  /* NORTH/SOUTH UPDATE FIELD */
  strcpy(txt, "NORTH/SOUTH");
  drawtext  (XTXT, NS_Txt_Y  , txt, NS_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, NS_Cir_Y-8, 60, 18, NS_Color);
  drawCircle(XRED, NS_Cir_Y  , Circle_Size, ST7735_RED);
  drawCircle(XYEL, NS_Cir_Y  , Circle_Size, ST7735_YELLOW);
  fillCircle(XGRN, NS_Cir_Y  , Circle_Size, ST7735_GREEN);
  drawtext  (XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2);
    
  /* NORTH/SOUTH LEFT TURN UPDATE FIELD */
  strcpy(txt, "N/S LT");
  drawtext  (XTXT, NSLT_Txt_Y, txt, NSLT_Color, ST7735_BLACK, TS_1);
  drawRect  (XTXT, NSLT_Cir_Y-8, 60, 18, NSLT_Color);
  fillCircle(XRED, NSLT_Cir_Y, Circle_Size, ST7735_RED);
  drawCircle(XYEL, NSLT_Cir_Y, Circle_Size, ST7735_YELLOW);
  drawCircle(XGRN, NSLT_Cir_Y, Circle_Size, ST7735_GREEN);  
  drawtext  (XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);
  

  /* EAST/WEST PEDESTRIAM UPDATE FIELD */  
  drawtext(2, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);
  strcpy(txt, "PEW");  
  drawtext(3, EW_Txt_Y, txt, EW_Color, ST7735_BLACK, TS_1);

  /* NORTH/SOUTH PEDESTRIAM UPDATE FIELD */
  strcpy(txt, "PNS");  
  drawtext(3, NS_Txt_Y, txt, NS_Color, ST7735_BLACK, TS_1);
  drawtext(2, PED_NS_Count_Y, PED_NS_Count, NS_Color, ST7735_BLACK, TS_2);
    
  /* MISCELLANEOUS UPDATE FIELD */  
  strcpy(txt, "EWP EWLT NSP NSLT MD");
  drawtext(1,  Switch_Txt_Y, txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(6,  Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(32, Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(58, Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  drawtext(87, Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
  
  drawtext(112,Switch_Txt_Y+9, SW_MODE_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
}

void Do_Init()                                                                  // Initialize the ports
{
 init_UART();                                                                   // Initialize the uart
 Init_ADC();                                                                    // Initiailize the ADC with the
                                                                                // programming of ADCON1
 OSCCON=0x60;                                                                   // Set oscillator to 8 MHz

                                                                                // that all the INTx pins are
                                                                                // inputs
INTCONbits.INT0IF = 0 ;                                                         // Clear INT0IF
 INTCON3bits.INT1IF = 0;                                                        // Clear INT1IF
 INTCON3bits.INT2IF =0;                                                         // Clear INT2IF
INTCON2bits.INTEDG0=0 ;                                                         // INT0 EDGE falling
INTCON2bits.INTEDG1=0;                                                          // INT1 EDGE falling
INTCON2bits.INTEDG2=1;                                                          // INT2 EDGE rising
INTCONbits.INT0IE =1;                                                           // Set INT0 IE
 INTCON3bits.INT1IE=1;                                                          // Set INT1 IE
 INTCON3bits.INT2IE=1;                                                          // Set INT2 IE
 INTCONbits.GIE=1;                                                              // Set the Global Interrupt Enable
} 

void interrupt high_priority chkisr()                                           
{
if (INTCONbits.INT0IF == 1) INT0_ISR();                                         // check if INT0 has occured
if (INTCON3bits.INT1IF == 1) INT1_ISR();                                        //checks INT1
if (INTCON3bits.INT2IF == 1) INT2_ISR();                                        //checks INT2
}
void INT0_ISR()                                                                 
{
     INTCONbits.INT0IF=0;                                                       // Clear the interrupt flag
     if (MODE==1)                                                               //interrupt flag only changes in day mode
     {
         NS_PED_SW = 1;                                                         // set software interrupt flag, which is NS_PED_SW
         
     }
     else
     {
         NS_PED_SW = 0;                                                         //clears NS_PED_SW
     }
}

void INT1_ISR()
{
     
     INTCON3bits.INT1IF=0;                                                      // Clear the interrupt flag
     if (MODE==1){                                                              //interrupt flag only changes in day mode
         EW_PED_SW = 1;                                                         // set software interrupt flag, which is EW_PED_SW
         
     } 
     else
     {
         EW_PED_SW = 0;                                                         //clears EW_PED_SW
     }
} 

void INT2_ISR()
{
     INTCON3bits.INT2IF=0;                                                      // Clear the interrupt flag
     FLASHING_REQUEST = 1;                                                      //sets software interrupt flag, FLASHING_REQUEST
} 


void main(void)
{
    Do_Init();                                                                  //initializes ADCON, clock, and interrupts
    init_IO();                                                                  //initializes IO
    
    RBPU = 0;    
    Initialize_Screen();                                                        // Initialize the TFT screen

    while(1)                                                                    // forever loop
    {
        
        int nStep = get_full_ADC();                                             // calculates the # of steps for analog conversion
        volt = nStep * 5 /1024.0;                                               // gets the voltage in Volts, using 5V as reference s instead of 4, also divide by 1024 
        printf("Voltage = %f \r\n", volt);
        SW_MODE = volt < 3.5 ? 1:0;                                             // Mode = 1, Day_mode, Mode = 0 Night_mode
        printf("%d", SW_MODE);
        if(FLASHING_REQUEST)                                                    //checks FLASHING_REQUEST flag, if true, FLASHING would start
        {
         FLASHING_REQUEST = 0;                                                  //FLASHING REQUEST is cleared
         FLASHING = 1;                                                          //FLASHING is set to 1
         Do_Flashing(FLASHING);                                                 //calls Do_Flashing
        }
        if (SW_MODE)    
        {
            Day_Mode();                                                         // calls Day_Mode() function
        }
        else                        
        {
            NS_PED_SW = 0;                                                      //resets NS and EW PED SW
            EW_PED_SW = 0;
            Night_Mode();                                                       // calls Night_Mode() function
            
        }
        
     
    } 
}

void init_IO()
{
    TRISA = 0x01;                                                               //Pin 1 of PORTA is input
    TRISB = 0x37;                                                               //Pin 0-2, 4-5 of PORTB is input
    TRISC = 0x00;                                                               //PORTC all outputs
    TRISD = 0x00;                                                               //PORTD all outputs
    TRISE = 0x00;                                                               //PORTE all outputs 
}

void Init_ADC()
{
    ADCON0 = 0x01;                                                              //sets AN0 as input, initialize AD subsystem
    ADCON1 = 0x0E;                                                              //sets AN0 as analog, with VSS=5V as reference
    ADCON2= 0XA9;                                                               //right justify the result. Set the bit conversion time (TAD) and acquisition time
}

unsigned int get_full_ADC()                                                     
{
unsigned int result;
   ADCON0bits.GO=1;                                                             // Start Conversion
   while(ADCON0bits.DONE==1);                                                   // wait for conversion to be completed
   result = (ADRESH * 0x100) + ADRESL;                                          // combine result of upper byte and
                                                                                // lower byte into result
   return result;                                                               // return the result.
}

void Set_NS(char color)                                                         //function that sets NS RGB color
{
    direction = NS;                                                             //direction is set NS
    update_LCD_color(direction, color);                                         //calls function to update NS on LCD
    
    switch (color)
    {
        case OFF: NS_RED =0;NS_GREEN=0;break;                                   // Turns off the NS LED
        case RED: NS_RED =1;NS_GREEN=0;break;                                   // Sets NS LED RED
        case GREEN: NS_RED =0;NS_GREEN=1;break;                                 // sets NS LED GREEN
        case YELLOW: NS_RED =1;NS_GREEN=1;break;                                // sets NS LED YELLOW
    }
}

void Set_NS_LT(char color)                                                      //function that sets NSLT RGB
{
    direction = NSLT;                                                           //direction is set NSLT
    update_LCD_color(direction, color);                                         //calls function to update NSLT on LCD
    
    switch (color)
    {
        case OFF: NSLT_RED =0;NSLT_GREEN=0;break;                               // Turns off the NSLT LED
        case RED: NSLT_RED =1;NSLT_GREEN=0;break;                               // Sets NSLT LED RED
        case GREEN: NSLT_RED =0;NSLT_GREEN=1;break;                             // sets NSLT LED GREEN
        case YELLOW: NSLT_RED =1;NSLT_GREEN=1;break;                            // sets NSLT LED YELLOW
    }
}

void Set_EW(char color)                                                         //function used to set EW RGB
{
    direction = EW;                                                             //sets direction as EW
    update_LCD_color(direction, color);                                         //updates EW on LCD
    
    switch (color)
    {
        case OFF: EW_RED =0;EW_GREEN=0;break;                                   // Turns off the EW LED
        case RED: EW_RED =1;EW_GREEN=0;break;                                   // Sets EW LED RED
        case GREEN: EW_RED =0;EW_GREEN=1;break;                                 // sets EW LED GREEN
        case YELLOW: EW_RED =1;EW_GREEN=1;break;                                // sets EW LED YELLOW
    }
}

void Set_EW_LT(char color)                                                      //function used to set EWLT RGB
{
    direction = EWLT;                                                           //sets direction as EWLT
    update_LCD_color(direction, color);                                         //updates EWLT on LCD
    
    switch (color)
    {
        case OFF: EWLT_RED =0;EWLT_GREEN=0;break;                               // Turns off the EWLT LED
        case RED: EWLT_RED =1;EWLT_GREEN=0;break;                               // Sets EWLT LED RED
        case GREEN: EWLT_RED =0;EWLT_GREEN=1;break;                             // sets EWLT LED GREEN
        case YELLOW: EWLT_RED =1;EWLT_GREEN=1;break;                            // sets EWLT LED YELLOW
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

void PED_Control( char direction, char Num_Sec)                                 //function used to control pedestrians
{ 
    
    PORTD = PORTD | 0x7f;                                                       //PORTD0-6 are set to 1 with bit 7 staying as its original value           
    for(char i = Num_Sec-1;i>0; i--)
    {
        PORTD = LED_7seg[i] & 0xFF;                                             //PORTD outputs the 7seg number to display on 7seg
        update_LCD_PED_Count(direction, i);                                     //LCD pedestrian counter is updated
        Wait_One_Second_With_Beep();                                            // hold the number on 7-Segment for 1 second
    }
    PORTD = LED_7seg[0] & 0xFF;                                                 //PORTD outputs 0 to 7seg to display a 0
    
    update_LCD_PED_Count(direction, 0);                                         //LCD pedestrian counter is set to 0
    Wait_One_Second_With_Beep();  
    
    if (direction == 2)                                                         //if direction is in NS, NS_PED_SW is reset
    {
        NS_PED_SW = 0;
    }
    if (direction == 0)                                                         //if direction is in EW, EW_PED_SW is reset
    {
        EW_PED_SW = 0;
    }
}

void Day_Mode()                                                                 //function used for day mode
{
    update_LCD_misc();                                                          //calls function used to update the misc parts of LCD
    MODE = 1;                                                                   // MODE is set to 1
    MODE_LED = 1;                                                               //mode led is turned on
    Act_Mode_Txt[0] = 'D';                                                      //LCD mode text displays as D
    
    Set_EW(RED);                                                                //Step 1
    Set_EW_LT(RED);                                                             //Sets NS as green and everything red
    Set_NS_LT(RED);
    Set_NS(GREEN);
    
    if(NS_PED_SW ==1)                                                           //Step 2
    {                                                                           //Read logic state of NSPED_SW, if true, calls PED_CONTROL
        PED_Control(2,7);                                                       //in NS direction for 7 seconds
    }
    
    Wait_N_Seconds(8);                                                          //Step 3 wait 8 sec with NS GREEN
    
    Set_NS(YELLOW);                                                             //Step 4 NS Yellow 2 sec
    Wait_N_Seconds(2);                              
    
    Set_NS(RED);                                                                //Step 5 NS RED    
    
    if (EW_LT_SW == 1)                                                          //Step 6 Checks logic state of EWLT_SW
    {                                                                           //if true, goes to step 7, if not skip to step 10
        Set_EW_LT(GREEN);                                                       //Step 7 Set EWLT GREEN 7 sec
        Wait_N_Seconds(7);
        
        Set_EW_LT(YELLOW);                                                      //Step 8 EWLT Yellow 2 sec
        Wait_N_Seconds(2);
        
        Set_EW_LT(RED);                                                         //Step 9 EWLT Red
    }
    
    Set_EW(GREEN);                                                              //Step 10 EW GREEN, Read EWPED_SW logic state
    if(EW_PED_SW == 1)                                                          //If true, PED_CONTROL is called in EW direction for 6 sec
    {
        PED_Control(0,6);
    }
    
    Set_EW(GREEN);                                                              //Step 11 EW green 6 sec
    Wait_N_Seconds(6);
    
    Set_EW(YELLOW);                                                             //Step 12 EW yellow 2 sec
    Wait_N_Seconds(2);
    
    Set_EW(RED);                                                                //Step 13 EW red
    
    
    if (NS_LT_SW == 1)                                                          //Step 14, check logic state of NSLT
    {                                                                           //if true, go to step 15, if not step 18
        Set_NS_LT(GREEN);                                                       //Step 15 NSLT GREEN 6 sec
        Wait_N_Seconds(6);
        
        Set_NS_LT(YELLOW);                                                      //Step 16 NSLT Yellow 2 sec
        Wait_N_Seconds(2);
        
        Set_NS_LT(RED);                                                         //Step 17 NSLT RED
    }                                                                           //Step 18 DONE
    
}
void Night_Mode()                                                               //function used to turn on Night mode
{ 
    MODE = 0;                                                                   //mode is set to 0
    MODE_LED = 0;                                                               //mode LED is turned off
    Act_Mode_Txt[0] = 'N';                                                      //mode on LCD is set to N
    
    Set_EW(RED);                                                                //Step 1 everything red, NS GREEN 5 sec
    Set_EW_LT(RED);
    Set_NS_LT(RED);
    Set_NS(GREEN);
    Wait_N_Seconds(5);
    
    Set_NS(YELLOW);                                                             //Step 2 NS Yellow 3 sec
    Wait_N_Seconds(3);                                                  
    Set_NS(RED);                                                                //Step 3 NS Red
    
    if (EW_LT_SW == 1)                                                          //Step 5 Check logic state of EWLT_SW, if false skip to Stepp 9
    {
        Set_EW_LT(GREEN);                                                       //Step 6 EWLT GREEN 6 sec
        Wait_N_Seconds(6);
        
        Set_EW_LT(YELLOW);                                                      //Step 7 EWLT YELLOW 3 sec
        Wait_N_Seconds(3);
        
        Set_EW_LT(RED);                                                         //Step 8 EWLT RED 
    }
    
    Set_EW(GREEN);                                                              //Step 9 EW GREEN 7 sec
    Wait_N_Seconds(7);
    
    Set_EW(YELLOW);                                                             //Step 10 EW YELLOW 3 sec
    Wait_N_Seconds(3);
    
    Set_EW(RED);                                                                //Step 11 EW RED
    
    if (NS_LT_SW == 1)                                                          //Step 12 check logic state of NSLT_SW, if false skip to 16
    {   
        Set_NS_LT(GREEN);                                                       //Step 13 NSLT GREEN 7 sec
        Wait_N_Seconds(7);
        
        Set_NS_LT(YELLOW);                                                      //Step 14 NSLT YELLOW 3 sec
        Wait_N_Seconds(3);
        
        Set_NS_LT(RED);                                                         //Step 15 NSLT RED
    }
                                                                                //Step 16 DONE

}

void Old_Wait_One_Second()                                                      //creates one second delay and blinking asterisk
{
    SEC_LED = 1;
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)
    SEC_LED = 0;
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)

}

void Wait_One_Second()                                                          //creates one second delay and blinking asterisk
{
    SEC_LED = 1;                                                                //second led is turned on
    strcpy(txt,"*");                                                            //displays a * on LCD
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);                        
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)

    SEC_LED = 0;                                                                //second led is turned off
    strcpy(txt," ");                                                            //displays a space to get rid of the *,to simulate a blinking every second
    drawtext(120,10,txt,ST7735_WHITE,ST7735_BLACK,TS_1);
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)
    update_LCD_misc();                                                          //updates misc parts of LCD
}

void Wait_One_Second_With_Beep()                                                // creates one second delay as well as sound buzzer
{
    SEC_LED = 1;                                                                // First, turn on the SEC LED
    Activate_Buzzer();                                                          // Activate the buzzer
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)
    SEC_LED = 0;                                                                // then turn off the SEC LED
    Deactivate_Buzzer ();                                                       // Deactivate the buzzer
    Wait_Half_Second();                                                         // Wait for half second (or 500 msec)
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

void Wait_N_Seconds (char seconds)
{
    char I;         
    for (I = seconds; I> 0; I--)
    {
        PORTD = LED_7seg[I] & 0xFF;                                             //displays seconds left waiting on 7seg
        update_LCD_count(direction, I);                                         //and LCD
        Wait_One_Second();                                                      // calls Wait_One_Second for x number of times
    }
    update_LCD_count(direction, 0);                                             //LCD counter is set to 0
}

void update_LCD_color(char direction, char color)
{
    char Circle_Y;
    Circle_Y = EW_Cir_Y + direction * 30;    
    
    if (color == Color_Off)                                                     //if Color off make all circles black but leave outline
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);                       
    }    
    
    if (color == Color_Red)                                                     //if the color is red only fill the red circle with red
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);  
    }
          
    if (color == Color_Yellow)                                                  //if the color is red only fill the red circle with red
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_BLACK); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);  
    }
    
    if (color == Color_Green)                                                   //if the color is red only fill the red circle with red
    {
        fillCircle(XRED, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XYEL, Circle_Y, Circle_Size, ST7735_BLACK);
        fillCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN); 
        drawCircle(XRED, Circle_Y, Circle_Size, ST7735_RED);            
        drawCircle(XYEL, Circle_Y, Circle_Size, ST7735_YELLOW);
        drawCircle(XGRN, Circle_Y, Circle_Size, ST7735_GREEN);  
    }
}

void update_LCD_count(char direction, char count)
{
   switch (direction)                                                           //update traffic light no ped time
   {
      case EW:                                                                  //depending on the case, the corresponding direction on LCD
        EW_Count[0] = count/10  + '0';                                          //will update its counter
        EW_Count[1] = count%10  + '0';
        drawtext(XCNT, EW_Count_Y, EW_Count, EW_Color, ST7735_BLACK, TS_2);                
        break;
      
      case EWLT:        
        EWLT_Count[0] = count/10  + '0';
        EWLT_Count[1] = count%10  + '0';
        drawtext(XCNT, EWLT_Count_Y, EWLT_Count, EWLT_Color, ST7735_BLACK, TS_2);                
        break;
        
    case NS:        
        NS_Count[0] = count/10  + '0';
        NS_Count[1] = count%10  + '0';
        drawtext(XCNT, NS_Count_Y, NS_Count, NS_Color, ST7735_BLACK, TS_2);                
        break;
        
    case NSLT:        
        NSLT_Count[0] = count/10  + '0';
        NSLT_Count[1] = count%10  + '0';
        drawtext(XCNT, NSLT_Count_Y, NSLT_Count, NSLT_Color, ST7735_BLACK, TS_2);                
        break;
    }  
}

void update_LCD_PED_Count(char direction, char count)
{
   switch (direction)                                                           //depending on the case, the corresponding direction PED counter
   {                                                                            //on the LCD will update
      case EW:       
        PED_EW_Count[0] = count/10  + '0';                                      // PED count upper digit
        PED_EW_Count[1] = count%10  + '0';                                      // PED Lower
        drawtext(PED_Count_X, PED_EW_Count_Y, PED_EW_Count, EW_Color, ST7735_BLACK, TS_2);        
        break;
     
    case NS:       
        PED_NS_Count[0] = count/10  + '0';                                      // PED count upper digit
        PED_NS_Count[1] = count%10  + '0';                                      // PED Lower
        drawtext(PED_Count_X, PED_NS_Count_Y, PED_NS_Count, EW_Color, ST7735_BLACK, TS_2);        
        break;

   }
   
}

void update_LCD_misc()
{
    int nStep = get_full_ADC();                                                 // calculates the # of steps for analog conversion
    volt = nStep * 5 /1024.0;                                                   // gets the voltage in Volts, using 5V as reference s instead of 4, also divide by 1024 
    SW_MODE = volt < 3.5 ? 1:0;                                                 // Mode = 1, Day_mode, Mode = 0 Night_mode
    
    SW_EWPED = EW_PED_SW;
    SW_EWLT = EW_LT_SW;    
    SW_NSPED = NS_PED_SW;
    SW_NSLT = NS_LT_SW;

    if (SW_EWPED == 0) SW_EWPED_Txt[0] = '0'; else SW_EWPED_Txt[0] = '1';       // Set Text at bottom of screen to switch states
    if (SW_EWLT == 0) SW_EWLT_Txt[0] = '0'; else SW_EWLT_Txt[0] = '1';
    if (SW_NSPED == 0) SW_NSPED_Txt[0] = '0'; else SW_NSPED_Txt[0] = '1';
    if (SW_NSLT == 0) SW_NSLT_Txt[0] = '0'; else SW_NSLT_Txt[0] = '1';
    if (FLASHING_REQUEST == 0) FlashingR_Txt[0] = '0'; else FlashingR_Txt[0] = '1'; //sets text at FR and FS at top of screen
    if (FLASHING == 0) FlashingS_Txt[0] = '0'; else FlashingS_Txt[0] = '1';
    
    if (SW_MODE == 0) SW_MODE_Txt[0]= 'N'; else SW_MODE_Txt[0] = 'D'; 
    
    drawtext(35,    10,             Act_Mode_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);    
    drawtext(6,   Switch_Txt_Y+9, SW_EWPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(32,  Switch_Txt_Y+9, SW_EWLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(58,  Switch_Txt_Y+9, SW_NSPED_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);       // Show switch and sensor states at bottom of the screen
    drawtext(87,  Switch_Txt_Y+9, SW_NSLT_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);

    drawtext(70,   10, FlashingR_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    drawtext(100,  10, FlashingS_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    
    drawtext(112,  Switch_Txt_Y+9, SW_MODE_Txt, ST7735_WHITE, ST7735_BLACK, TS_1);
    
    
}

void Do_Flashing()                                                              //when there is a flashing request , Do_Flashing is called
{
    
    while(FLASHING)                                                             //loops when flashing is true
    {
        if(FLASHING_REQUEST)                                                    //if FLASHING_REQUEST is true, FLASHING_REQUEST and FLASHING is reset
        {                                                                       //exits while loop
            FLASHING_REQUEST = 0;
            FLASHING = 0;
            
        }
        else
        {                                                                       //if FLASHING_REQUEST is not true, LEDs and stoplights on LCD will
            Set_EW(RED);                                                        //blink red every second
            Set_EW_LT(RED);                                                      
            Set_NS_LT(RED);
            Set_NS(RED);
            Wait_One_Second();
            Set_EW(OFF);                                                        
            Set_EW_LT(OFF);                                                      
            Set_NS_LT(OFF);
            Set_NS(OFF);
            Wait_One_Second();
        }
        
    }
    

}



