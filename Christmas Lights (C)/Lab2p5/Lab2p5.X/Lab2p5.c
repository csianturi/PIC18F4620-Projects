#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#include <p18f4620.h>
#pragma config OSC = INTIO67
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config BOREN = OFF

void Delay_One_Sec()
{
 for(int I=0; I <17000; I++);           // Delay function used to add a 1 second delay
 }                                      // in between the LED switching colors

void main()
{
char in;                                                                // Use variable ?in? as char
 TRISA = 0xff;                                                          // set TRISA = binary 1111 1111 so PORTA becomes an input
 TRISB = 0x00;                                                          // set TRISB = binary 0000 0000 so PORTB becomes an output
 TRISC = 0x00;                                                          // set TRISC = binary 0000 0000 so PORTC becomes an output
 TRISD = 0x00;                                                          // set TRISD = binary 0000 0000 so PORTD becomes an output
 TRISE = 0x00;                                                          // set TRISE = binary 0000 0000 so PORTE becomes an output
 ADCON1 = 0x0F;                                                         // setup ports A and B in digital mode instead of analog

           
 int array2 [8] = {0x06, 0x07, 0x00, 0x02, 0x03, 0x04, 0x05,0x01};      // stores LED D2's sequence in an array
 int array3 [8] = {0x02, 0x07, 0x04, 0x00, 0x03, 0x01, 0x06, 0x05};     // stores LED D3's sequence in an array
 
   while (1)                                                            // infinite loop used to output the list of colors to the LED's
    {
        for(int i=0; i <8; i++)      
        { 
            PORTB = i;                                                  // PORTB has a normal sequence
            PORTD = array2[i];                                          // PORT D and E uses the arrays to output their own sequence
            PORTE = array3[i];
            Delay_One_Sec();         
        }
    }
}