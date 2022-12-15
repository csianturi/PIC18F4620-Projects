#include <stdio.h>
#include <stdlib.h>
#include <xc.h>

#include <p18f4620.h>
#include "utils.h"

extern char found;
extern char Nec_code1;
extern short nec_ok;
extern char array1[21];
extern char duty_cycle;

char check_for_button_input()                                                   //checks for button input
{       
    if (nec_ok == 1)                                                            //if a button press is detected, sets found key
    {
            nec_ok = 0;


            INTCONbits.INT0IE = 1;                                              // Enable external interrupt
            INTCON2bits.INTEDG0 = 0;                                            // Edge programming for INT0 falling edge

            found = 0xff;
            for (int j=0; j< 21; j++)
            {
                if (Nec_code1 == array1[j]) 
                {
                    found = j;
                    j = 21;
                }
            }
            
            if (found == 0xff) 
            {
                printf ("Cannot find button \r\n");
                return (0);
            }
            else
            {
                return (1);
            }
    }
}

char bcd_2_dec (char bcd)                                                       //function used to convert bcd->dec
{
    int dec;                                                                    
    dec = ((bcd>> 4) * 10) + (bcd & 0x0f);
    return dec;
}

int dec_2_bcd (char dec)                                                        //function used to convert dec->bcd
{
    int bcd;
    bcd = ((dec / 10) << 4) + (dec % 10);
    return bcd;
}

void Do_Beep()                                                                  //does a normal beep for 1sec
{
//    KEY_PRESSED = 1;
    Activate_Buzzer();
    Wait_One_Sec();
//    KEY_PRESSED = 0;
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Good()                                                             //does a 2KHz beep for 1 sec
{
    Activate_Buzzer_2KHz();
    Wait_One_Sec();
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);
}

void Do_Beep_Bad()                                                              //does a 500Hz low pitch beep for 1 sec
{
    Activate_Buzzer_500Hz();
    Wait_One_Sec();
    Deactivate_Buzzer();
    do_update_pwm(duty_cycle);

}

void Wait_One_Sec()                                                             //function used to wait one second
{
    for (int k=0;k<0x8000;k++);
}

void Activate_Buzzer()                                                          //used to activate buzzer by setting CCP modules
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b01001010 ;
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_500Hz()                                                    //used to activate buzzer by setting CCP
{
    PR2 = 0b11111001 ;
    T2CON = 0b00000111 ;
    CCPR2L = 0b01111100 ;   
    CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_2KHz()                                                     //used to activate buzzer by setting CCP
{
PR2 = 0b11111001 ;
T2CON = 0b00000101 ;
CCPR2L = 0b01111100 ;
CCP2CON = 0b00111100 ;
}

void Activate_Buzzer_4KHz()                                                     //used to activate buzzer by setting CCP
{
    PR2 = 0b01111100 ;
    T2CON = 0b00000101 ;
    CCPR2L = 0b00111110 ;
    CCP2CON = 0b00011100 ;
}

void Deactivate_Buzzer()                                                        //used to deactivate the signal for the buzzer
{
    CCP2CON = 0x0;
	PORTBbits.RB3 = 0;
}

void do_update_pwm(char duty_cycle)                                             //updates the pwm of the fan
{ 
	float dc_f;
	int dc_I;
	PR2 = 0b00000100 ;                                                          // Set the frequency for 25 Khz 
	T2CON = 0b00000111 ;                                                        // As given in website
	dc_f = ( 4.0 * duty_cycle / 20.0) ;                                         // calculate factor of duty cycle versus a 25 Khz signal
	dc_I = (int) dc_f;                                                          // Truncate integer
	if (dc_I > duty_cycle) dc_I++;                                              // Round up function
	CCP1CON = ((dc_I & 0x03) << 4) | 0b00001100;
	CCPR1L = (dc_I) >> 2;
}

void Set_RGB_Color(char color)                                                  //sets the alarm RGB color
{
    PORTE = color;
}

float read_volt()                                                               //function used to read the voltage at selected ADC channel
{
    int num_step = get_full_ADC();                                              //Calls get_full_ADC() and returns the number of steps)
    float voltage_t = num_step*0.004;
    return voltage_t;
}

unsigned int get_full_ADC()                                                     
{
    int result;
    ADCON0bits.GO=1;                                                            // Start Conversion
    while(ADCON0bits.DONE==1);                                                  // wait for conversion to be completed
    result = (ADRESH * 0x100) + ADRESL;                                         // combine result of upper byte and
                                                                                // lower byte into result
    return result;                                                              // return the result.
}

void Init_ADC()
{
    ADCON0=0x01;                                                                // select channel AN0, and turn on the ADC subsystem
    ADCON1=0x0E;                                                                // select pins AN0 as analog signal, VDD-VSS as
                                                                                // reference voltage
    ADCON2=0xA9;                                                                // right justify the result. Set the bit conversion time (TAD) and
                                                                                // acquisition time
}


