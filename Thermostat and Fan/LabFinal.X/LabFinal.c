#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#include <usart.h>
#include <string.h>

#include "I2C_Soft.h"
#include "I2C_Support.h"
#include "Interrupt.h"
#include "Fan_Support.h"
#include "Main.h"
#include "ST7735_TFT.h"
#include "utils.h"

#pragma config OSC = INTIO67
#pragma config BOREN =OFF
#pragma config WDT=OFF
#pragma config LVP=OFF
#pragma config CCP2MX = PORTBE

void Test_Alarm();
char second = 0x00;
char minute = 0x00;
char hour = 0x00;
char dow = 0x00;
char day = 0x00;
char month = 0x00;
char year = 0x00;

char found;
char tempSecond = 0xff; 
signed int DS1621_tempC, DS1621_tempF;
char setup_second, setup_minute, setup_hour, setup_day, setup_month, setup_year;
char alarm_second, alarm_minute, alarm_hour, alarm_date;
char setup_alarm_second, setup_alarm_minute, setup_alarm_hour;
unsigned char heater_set_temp = 75;
unsigned char Nec_state = 0;
float volt;
char INT1_flag, INT2_flag;

short nec_ok = 0;
char Nec_code1;
char HEATER;
char duty_cycle;
int rps;
int rpm;
int ALARMEN;
int alarm_mode, MATCHED,color;


char buffer[31]     = " ECE3301L Fl'22 S3 \0";
char *nbr;
char *txt;
char tempC[]        = "+25";
char tempF[]        = "+77";
char time[]         = "00:00:00";
char date[]         = "00/00/00";
char alarm_time[]   = "00:00:00";
char Alarm_SW_Txt[] = "OFF";
char Heater_Set_Temp_Txt[] = "075F";
char Heater_SW_Txt[]   = "OFF";                                                 // text storage for Heater Mode
char array1[21]={0xa2,0x62,0xe2,0x22,0x02,0xc2,0xe0,0xa8,0x90,0x68,0x98,0xb0,0x30,0x18,0x7a,0x10,0x38,0x5a,0x42,0x4a,0x52};

    
char DC_Txt[]       = "000";                                                    // text storage for Duty Cycle value
char Volt_Txt[]     = "0.00V";
char RTC_ALARM_Txt[]= "0";                      
char RPM_Txt[]      = "0000";                                                   // text storage for RPM

char setup_time[]       = "00:00:00";
char setup_date[]       = "01/01/00";
char setup_alarm_time[] = "00:00:00"; 
char setup_heater_set_text[]   = "075F";


void putch (char c)                                                             //for printf
{   
    while (!TRMT);       
    TXREG = c;
}

void init_UART()                                                                //for tera term
{
    OpenUSART (USART_TX_INT_OFF & USART_RX_INT_OFF & USART_ASYNCH_MODE & USART_EIGHT_BIT & USART_CONT_RX & USART_BRGH_HIGH, 25);
    OSCCON = 0x70;
}



void Do_Init()                                                                  // Initialize the ports 
{ 
    init_UART();                                                                // Initialize the uart
    Init_ADC();                                                                 //initialize ADC
    OSCCON=0x70;                                                                // Set oscillator to 8 MHz 
    
    ADCON1=0x0E;                                                                //sets AN0 as analog
    TRISA = 0x11;                                                               //initializes IO ports
    TRISB = 0x07;
    TRISC = 0x01;
    TRISD = 0x00;
    TRISE = 0x00;
    PORTE = 0x00;

    HEATER = 0;                                                                 //sets heater as off
    RBPU=0;                                             

    TMR3L = 0x00;                                                               //initializes T3 modules
    T3CON = 0x03;
    I2C_Init(100000); 

    DS1621_Init();                                                              //initializes DS1621
    Init_Interrupt();                                                           //initialize interrupts
    Turn_Off_Fan();                                                             //turns the fan off
    heater_set_temp = 75;                                                       //sets default heater set temp
}


void main() 
{
    Do_Init();                                                                  // Initialization  
    Initialize_Screen();                                                        
    DS3231_Turn_Off_Alarm();                                                    //start with alarm off
    DS3231_Read_Alarm_Time();                                                   // Read alarm time for the first time

    tempSecond = 0xff;                                                          
    while (1)
    {
        DS3231_Read_Time();                                                     //reads the time from DS3231

        if(tempSecond != second)                                                //checks if time has changed every second
        {
            tempSecond = second;                                                //sets new time
            rpm = get_RPM();                                                    //gets current rpm of fan
            volt = read_volt();                                                 //reads current voltage from light sensor
            DS1621_tempC = DS1621_Read_Temp();                                  //reads current temp from DS1621
            if ((DS1621_tempC & 0x80) == 0x80)                                  //converts the temp from C to F
                DS1621_tempC = - (0x80 - DS1621_tempC & 0x7f);
            DS1621_tempF = (DS1621_tempC * 9 / 5) + 32;

            printf ("%02x:%02x:%02x %02x/%02x/%02x",hour,                       //prints current time
                    minute,second,month,day,year);
            printf (" Temp = %d C = %d F ", DS1621_tempC, DS1621_tempF);        //prints current tempurature
            //printf ("alarm = %d Heater = %d ", RTC_ALARM_NOT, HEATER);
            printf ("RPM = %d  dc = %d\r\n", rpm, duty_cycle);                  //prints current fan rpm
            
            Set_RPM_RGB(rpm);                                                   //sets the RPM RGB color based on rpm
            Set_DC_RGB(duty_cycle);                                             //sets the DC RGB color based on duty cycle
            Monitor_Heater();                                                   //monitors heater status
            Test_Alarm();                                                       //checks if alarm time is reached
            Update_Screen();                                                    //updates the screen
        }
        
        if (check_for_button_input() == 1)                                      //checks for specific button inputs
        {                                                                       

            nec_ok = 0;
            switch (found)                                                      
            {
                case Ch_Minus:                                                  //if Ch_Minus, goes the time setup screen
                    Do_Beep_Good();
                    Do_Setup_Time();
                    break;
                
                case Channel:                                                   //if Channel, goes to alarm setup screen
                    Do_Beep_Good();
                    Do_Setup_Alarm_Time();
                    break;
                    
                case Ch_Plus:                                                   //if Ch_plus, goes to heater setup screen
                    Do_Beep_Good();
                    Do_Setup_Heater_Temp();            
                    break;
                    
                case Play_Pause:                                                //if Play_Pause, turns on the heater
                    Do_Beep_Good();
                    Toggle_Heater();
                     break;           
        
                default:
                    Do_Beep_Bad();                                              //if none of these are pressed, do bad beep
                    break;
            }
        }    

        
        if (INT1_flag == 1)                                                     //cheecks for the alarm INT flag, if there is an INT
        {                                                                       //toggles the alarm enable
            INT1_flag = 0;
            ALARMEN = (!ALARMEN);
        }
        
        
    }
}

void Test_Alarm()                                                               //function used to check alarm
{
    if (alarm_mode == 0 & ALARMEN == 1)                                         //if there is an enable but is not in alarm mode
    {
        DS3231_Turn_On_Alarm();                                                 //turns on the alarm on the DS3231
        alarm_mode = 1;                                                         //and change it to alarm mode
    }
    else if (alarm_mode == 1 & ALARMEN == 0)                                    //if it is alarm mode, but no enable
    {
        DS3231_Turn_Off_Alarm();                                                //turns off the alarm on the DS3231
        alarm_mode = 0;                                                         //leave alarm mode
        Set_RGB_Color(0);                                                       //turn off RGB
        Deactivate_Buzzer();                                                    //turn of the buzzer
        
    }
    else if (alarm_mode == 1 & ALARMEN == 1)                                    //if both in alarm mode and alarm enabled
    {
        if(RTC_ALARM_NOT == 0)                                                  //checks for the 0 signal from DS3231, which is matched signal if 
        {                                                                       //current time equals alarm time
            MATCHED = 1;                                                        //sets MATCHED as 1
            Activate_Buzzer_4KHz();                                             //activate a high pitched beep
            
        }
        if (MATCHED == 1)                                                       //if matched
        {   
            Set_RGB_Color(color++);                                             //sets RGB color as initial off, 
            if (color >7) color = 0;                                            //but increments to 7 then resetting to 0, starting at red to white to off
            if (volt > 2.5)                                                     //checks if light sensor detects darkness to turn off alarm
            {
                MATCHED = 0;                                                    //matched is back to 0
                color = 0;                                                      //
                Set_RGB_Color(color);                                           //RGB is turned off
                Deactivate_Buzzer();                                            //deactivates the buzzer
                do_update_pwm(duty_cycle);                                      //updates the pwm of fan
                alarm_mode = 0;                                                 //the alarm mode is turned off
            }
                
        }
    }
}






