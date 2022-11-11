; THIS FIRST ASSEMBLY LANGUAGE PROGRAM WILL FLASH LEDS
; CONNECTED TO THE PINS 0 THROUGH 3 OF PORT c
#include<P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
; Constant declarations
InA equ 0x22			    ;sets InA as memory location 0x22
InB equ 0x24			    ;sets InB as memory location 0x24
Result equ 0x26			    ;sets Result as memory location 0x26
ORG 0x0000
 ; CODE STARTS FROM THE NEXT LINE
START:
    MOVLW 0x0F ;
    MOVWF ADCON1		    ;Set all ports into digital mode instead of analog
    
    MOVLW 0xFF ;
    MOVWF TRISA			    ;Set PORTA as an input
    MOVLW 0xFF
    MOVWF TRISB			    ;set PORTB as an input
    MOVLW 0x07
    MOVWF TRISD			    ;set PORTD pins 0-2 as inputs and the rest as outputs
    
    MOVLW 0x00
    MOVWF TRISC			    ;set PORTC as outputs
    MOVLW 0x00
    MOVWF TRISE			    ;set PORTE as outputs

MAIN_LOOP:
    BTFSC PORTD, 2		    ;checks the pin 2 of PORTD
    GOTO PORTD2EQ1		    ;if pin 2 = 1 goes to subroutine PORTD2EQ1
    GOTO PORTD2EQ0		    ;if pin 2 = 0 goes to subroutine PORTD2EQ0
    
    
PORTD2EQ1:
    GOTO TASK_BCD		    ;goes to susbroutine TASK_BCD
    
PORTD2EQ0:
    BTFSC PORTD, 1		    ;checks pin 1 of PORTD
    GOTO PORTD21EQ01		    ;if pin 1 = 1 goes to subroutine PORTD21EQ01
    GOTO PORTD21EQ00		    ;if pin 1 = 0 goes to subroutine PORTD21EQ00

PORTD21EQ01:
    BTFSC PORTD, 0		    ;checks pin 0 of PORTD
    GOTO TASK_XOR		    ;if pin 0 = 1 goes to subroutine TASK_XOR
    GOTO TASK_OR		    ;if pin 0 = 0 goes to subroutine TASK_OR
    
PORTD21EQ00:
    BTFSC PORTD, 0		    ;checks pin 0 of PORTD
    GOTO TASK_ADD		    ;if pin 0 = 1 goes to subroutine TASK_ADD
    GOTO TASK_COMP		    ;if pin 0 = 0 goes to subroutine TASK_COMP


    
TASK_COMP:
    BCF PORTD, 6		    ;clears pin 6 of PORTD
    BCF PORTD, 5		    ;clears pin 5 of PORTD
    BCF PORTD, 4		    ;clears pin 4 of PORTD, should set LED as off
    CALL SUBROUTINE_COMP	    ;calls subroutine SUBROUTINE_COMP
    GOTO MAIN_LOOP		    ;goes back to top of MAIN_LOOP

TASK_ADD:
    BCF PORTD, 6		    ;clears pin 6 of PORTD
    BCF PORTD, 5		    ;clears pin 5 of PORTD
    BSF PORTD, 4		    ;sets pin 4 of PORTD =1, LED is red
    CALL SUBROUTINE_ADD		    ;calls SUBROUTINE_ADD
    GOTO MAIN_LOOP		    ;goes back to top of MAIN_LOOP
    
TASK_OR:
    BCF PORTD, 6		    ;clears pin 6 of PORTD
    BSF PORTD, 5		    ;sets pin 5 of PORTD = 1
    BCF PORTD, 4		    ;clears pin 4 of PORTD, LED is green
    CALL SUBROUTINE_OR		    ;calls SUBROUTINE_OR
    GOTO MAIN_LOOP		    ;goes back to top of MAIN_LOOP
    
TASK_XOR:
    BCF PORTD, 6		    ;clears pin 6 of PORTD
    BSF PORTD, 5		    ;sets pin 5 of PORTD = 1
    BSF PORTD, 4		    ;sets pin 4 of PORTD = 1, LED is yellow
    CALL SUBROUTINE_XOR		    ;calls SUBROUTINE_OR
    GOTO MAIN_LOOP		    ;goes back to top of MAIN_LOOP
    
TASK_BCD:
    BSF PORTD, 6		    ;sets pin 6 of PORTD = 1
    BCF PORTD, 5		    ;clears pin 5 of PORTD
    BCF PORTD, 4		    ;clears pin 4 of PORTD, LED is blue
    CALL SUBROUTINE_BCD		    ;calls SUBROUTINE_BCD
    GOTO MAIN_LOOP		    ;goes back to top of MAIN_LOOP

    
SUBROUTINE_COMP:
    MOVF PORTB, W		    ;copies contents of PORTB into W
    ANDLW 0x0F			    ;ANDS W with 0x0F, to mask in lower 4 bits
    MOVWF InB			    ;copies contents of W into InB
    COMF InB, W			    ;complements InB with W
    ANDLW 0x0F			    ;ANDS W with 0x0F, to mask in lower 4 bits
    MOVWF Result		    ;copies contents of W into Result
    MOVFF Result, PORTC		    ;takes contents of Result and outputs it to PORTC
    BZ ZERO_CHECK1		    ;if zero bit is 1 program will branch to ZERO_CHECK1, if not skips line
    GOTO ZERO_RESET1		    ;goes to ZERO_RESET1
	
    
    ZERO_CHECK1:
	BSF PORTE, 1		    ;sets pin 1 or PORTE = 1
	GOTO DONE1		    ;goes to DONE1
    
    ZERO_RESET1:
	BCF PORTE, 1		    ;clears pin 1 of PORTE
	GOTO DONE1		    ;goes to DONE1
    
    DONE1:
	RETURN			    
    
SUBROUTINE_ADD:
    MOVF PORTA, W		    ;takes input of PORTA and stores it in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InA			    ;copies contents of W into InA
    
    
    MOVF PORTB, W		    ;takes input of PORTB and stores it in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InB			    ;copies contents of W into InB
	
    MOVF InB, W			    ;copies contents of InB into W
    ADDWF InA, W		    ;adds contents of InA with W, and stores result in W
       
    MOVWF Result		    ;copies contents of W into Result
    MOVFF Result, PORTC		    ;outputs contents of Result into PORTC
    
    BZ ZERO_CHECK2		    ;if zero bit = 1 branches to ZERO_CHECK2, if not skips line
    GOTO ZERO_RESET2		    ;goes to ZERO_RESET2
    
    ZERO_CHECK2:
	BSF PORTE, 1		    ;sets pin 1 of PORTE = 1
	GOTO DONE2		    ;goes to DONE2
    
    ZERO_RESET2:
	BCF PORTE, 1		    ;clears pin 1 of PORTE = 1
	GOTO DONE2		    ;goes to DONE2
    
    DONE2:
	RETURN
    
SUBROUTINE_OR:
    MOVF PORTA, W		    ;takes input from PORTA and stores in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InA			    ;copies contents of W into InA
	
    
    MOVF PORTB, W		    ;takes input from PORTB and stores in W
    ANDLW 0x0F			    ;ANDW with 0x0F, to mask in lower 4 bits
    MOVWF InB			    ;copies contents of W into InB
    
    MOVF InB, W			    ;copies contents of InB into W
    IORWF InA, W		    ;OR contents of InA with W, stores result in W
       
    MOVWF Result		    ;copies contents of W into Result
    MOVFF Result, PORTC		    ;outputs Result with PORTC
    
    BZ ZERO_CHECK3		    ;if zero bit = 1, branch to ZERO_CHECK3, if not skips line
    GOTO ZERO_RESET3		    ;goes to ZERO_RESET3
    
    ZERO_CHECK3:
	BSF PORTE, 1		    ;sets pin 1 of PORT = 1
	GOTO DONE3		    ;goes to DONE3
    
    ZERO_RESET3:
	BCF PORTE, 1		    ;clears pin 1 of PORTE  
	GOTO DONE3		    ;goes to DONE3
    
    DONE3:
	RETURN
    
SUBROUTINE_XOR:
    MOVF PORTA, W		    ;takes input from PORTA and stores in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InA			    ;copies contents of W into InA
    
    
    MOVF PORTB, W		    ;takes input from PORTB and stores in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InB			    ;copies contents of W into InB
    
    MOVF InA, W			    ;copies contents of InA into W
    XORWF InB, W		    ;XOR W with contents of InB, stores result in W
       
    MOVWF Result		    ;copies contents of W into Result
    MOVFF Result, PORTC		    ;outputs contents of Result into PORTC
    
    BZ ZERO_CHECK4		    ;if zero bit = 1 branches to ZERO_CHECK4, if not skips line
    GOTO ZERO_RESET4		    ;goes to ZERO_RESET4
    
    ZERO_CHECK4:
	BSF PORTE, 1		    ;sets pin 1 of PORTE =1
	GOTO DONE4		    ;goes to DONE4
    
    ZERO_RESET4:
	BCF PORTE, 1		    ;clears pin 1 of PORTE
	GOTO DONE4		    ;goes to DONE4
    
    DONE4:
	RETURN
    
SUBROUTINE_BCD:
    MOVF PORTB, W		    ;takes input from PORTB and stores in W
    ANDLW 0x0F			    ;AND W with 0x0F, to mask in lower 4 bits
    MOVWF InB			    ;copies contents of InB into W
	
    MOVLW 0x0A			    ;places 0x0A into W (decimal 10)
    CPFSLT InB, W                   ;if InB > W, executes next line, if InB > W, skips next line
    GOTO ADD6			    ;goes to ADD6
    GOTO FINISH			    ;goes to FINISH
    
    ADD6:                    
	MOVF InB,W	    	    ;copies contents of InB into W
	ADDLW 0x06		    ;add W with 0x06 to convert to BCD
	MOVWF InB		    ;copies contents of W into InB
	
    FINISH:
	MOVFF InB,PORTC		    ;takes contents of InB and outputs to PORTC
    
    BZ ZERO_CHECK5		    ;if zero bit = 1 branches to ZERO_CHECK5
    GOTO ZERO_RESET5		    ;goes to ZERO_RESET5
    
    ZERO_CHECK5:
	BSF PORTE, 1		    ;sets pin 1 of PORTE =1
	GOTO DONE5		    ;goes to DONE5
    
    ZERO_RESET5:
	BCF PORTE, 1		    ;clears pin 1 of PORTE
	GOTO DONE5		    ;goes to DONE5
    
    DONE5:
	RETURN 

   
    
END






