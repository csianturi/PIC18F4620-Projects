
#include <p18f4620.h>
#include "Main.h"
#include "Fan_Support.h"
#include "stdio.h"

extern char HEATER;                                                             //extern variables created
extern char duty_cycle;
extern char heater_set_temp;
extern signed int DS1621_tempF;

int get_duty_cycle(signed int temp, int set_temp)                               //function used to calculate the duty cycle needed
{
    
    if(temp >= set_temp) duty_cycle = 0;
    else
    {
        signed int diff = set_temp - temp;
        
        if (diff > 50) duty_cycle = 100;                                        //depending on the difference between the set_temp and temp
        else if ( (25 <= diff) &                                                //will calculate the duty_cycle needed
                (diff < 50) ) duty_cycle = (set_temp - temp) * 2;               //boundaries are:
        else if ( (10 <= diff) &                                                //x>50
                (diff < 25) ) duty_cycle = (set_temp - temp) * (3/2);           //10<x<25
        else if ( (0 <= diff) & ( diff < 10) )                                  //0<x<10
        {
            duty_cycle = (set_temp - temp);
            //printf("%d\n", duty_cycle);
        }
    }
    return duty_cycle;
    

}

void Monitor_Heater()                                                           //function used to monitor the heater status
{   
    if(HEATER == 0 ) duty_cycle = 0;                                            //if heater is off, the duty cycle is set to 0
    else duty_cycle = get_duty_cycle(DS1621_tempF, heater_set_temp);            //else, actually calculate the duty cycle using get_duty_cycle
    
    do_update_pwm(duty_cycle);                                                  //updates the pwm using duty_cycle

    if (HEATER == 1)                                                            //if heater is on then will check ambient temp against the set temp
    {                                                                           //if ambient temp is less than the set temp, then the fan will turn on
        if(DS1621_tempF < heater_set_temp) Turn_On_Fan();                       //else will keep it off.
        else HEATER = 0;
    }
    else Turn_Off_Fan();
}

void Toggle_Heater()                                                            //toggles the heater enable variable
{
// add code just to flip the variable FAN
    HEATER= (!HEATER);
}

int get_RPM()
{
    int RPS = TMR3L / 2;                                                        // read the count. Since there are 2 pulses per rev
                                                                                // then RPS = count /2
    TMR3L = 0;                                                                  // clear out the count
    return (RPS * 60);                                                          // return RPM = 60 * RPS
}

void Turn_Off_Fan()
{
    HEATER = 0;                                                                 //sets heater=0, so it can be toggled on
    FAN_EN = 0;                                                                 //fan enable and led are turned off
    FAN_LED = 0;  

}

void Turn_On_Fan()
{
    do_update_pwm(duty_cycle);                                                  //reupdates the pwm
    HEATER = 1;                                                                 //sets heater=1, so it can be toggled off
    FAN_EN = 1;                                                                 //enable and led are turned on
    FAN_LED = 1;
}

void Set_DC_RGB(int duty_cycle)
{
        int temp = duty_cycle / 10;
        int output;
        if (temp ==0 )     output = 0;                                          //if the upper digit = 0 then LED2 will be off
        else if((temp >= 1) & (temp < 7))    output = temp;                     //if 1<=u<7 then DCRGB will display the color of the corresponding number of duty_cycle/10 
        else if (temp>=7) output = 0x07;                                        //if u>= 7, LED2 will be white
        PORTD = output | PORTD;                                                 //PORTD is OR'D by itself so RPMRGB wont be overwritten
}

void Set_RPM_RGB(int rpm)
{
    int output;                                                                 //create output variable
    if (rpm == 0) 
    {
        output == 0;                                                            //if rpm=0, RPMRGB will be off
        PORTD = output;
    }    
    else
    {
        int temp = rpm / 500;                                                   //rpm is divided by 500 to set corresponding colors with rpm ranges
        if (temp == 0) output = 0x01;                                           //if rpm is less than 500 but greater than 0, RPMRGB is red
        else if ( (temp >= 1) & (temp < 6) ) output = temp+1;                   //if rpm is in between 500 and 3000, RPMRGB would be the corresponding color
        else if ( temp >= 6) output = 0x07;                                     //if it is greater than 3000, RPMRGB is white
        PORTD = (output << 3);                                                  //output is shifted left 3 times so DCRGB wont be overwritten
    }
    
}





