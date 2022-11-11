#include <P18F4620.inc>
config OSC = INTIO67
config WDT = OFF
config LVP = OFF
config BOREN = OFF
                ; Constant declarations
Delay1 equ 0xFF
Delay2 equ 0xFF
Counter_L equ 0x20
Counter_H equ 0x21
Color_Value equ 0x28
Loop_Count equ 0x29

ORG 0x0000
                ; CODE STARTS FROM THE NEXT LINE
START:
ORG 0x0000
START:
    MOVLW 0x0F            
    MOVWF ADCON1		    ;sets all ports as digital instead of analog
    MOVLW 0x00			    
    MOVWF TRISB			    ;sets PORT B as outputs
    MOVLW 0x00            
    MOVWF TRISD			    ;sets PORT D as outputs
    MOVLW 0x00           
    MOVWF TRISE			    ;sets PORT E as outputs
    
				    ;Array 1 that will be outputted to PORTD
    MOVLW 0x20 
    MOVWF 0x50 
    
    MOVLW 0x70         
    MOVWF 0x51     
    
    MOVLW 0x40        
    MOVWF 0x52
    
    MOVLW 0x00        
    MOVWF 0x53
    
    MOVLW 0x30        
    MOVWF 0x54        
    
    MOVLW 0x10        
    MOVWF 0x55
    
    MOVLW 0x60        
    MOVWF 0x56
    
    MOVLW 0x50            
    MOVWF 0x57          
    
				    ;Array 2 that will be outputted to PORTE
    MOVLW 0x06            
    MOVWF 0x60
    
    MOVLW 0x07        
    MOVWF 0x61 
    
    MOVLW 0x00        
    MOVWF 0x62
    
    MOVLW 0x02        
    MOVWF 0x63 
    
    MOVLW 0x03        
    MOVWF 0x64
    
    MOVLW 0x04        
    MOVWF 0x65
    
    MOVLW 0x05        
    MOVWF 0x66
    
    MOVLW 0x01            
    MOVWF 0x67           
    
WHILE_LOOP:			    ;start of WHILE_LOOP   
    MOVLW 0x00			    ;inputs 0x00 to W 
    MOVWF Counter_L		    ;move contents of W into Counter_L
    MOVLW 0x08			    ;inputs 0x08 into W
    MOVWF Counter_H		    ;move contents of W into Counter_H
    LFSR 0, 0x50		    ;loads array 1 into fsr 0
    LFSR 1, 0x60		    ;loads array 2 into fsr 1

FOR_LOOP:			    ;start of FOR_LOOP
    MOVF POSTINC0, W		    ;takes contents of what FSR0 is pointing to and stores it into W,then increments by 1 and repeats
    MOVWF PORTD			    ;takes contents of W and outputs it in PORTD
    MOVF POSTINC1, W		    ;takes contents of what FSR1 is pointing  and stores it into W, then increments by 1 and repeats
    MOVWF PORTE			    ;takes contents of W and outputs it 
    MOVF Counter_L,W		    ;places value of Counter_L into W
    MOVWF PORTB			    ;takes contents of W and outputs to PORTB
    CALL DELAY_ONE_SEC		    ;delays 1 second
    INCF Counter_L,F		    ;increments Counter_L location by F
    DECF Counter_H ,F		    ;decrements Counter_H location by F
    BNZ FOR_LOOP		    ;if zero bit=0 then branch back to top of FOR_LOOP, if not, skips this line
    GOTO WHILE_LOOP		    ;goes to top of WHILE_LOOP

DELAY_ONE_SEC:
    MOVLW Delay1		    ;moves constant Delay1 into W
    MOVWF Color_Value		    ;moves contents of W into Color_Value
    
LOOP_OUTER:
    NOP				    ;stalls 1 cycle
    MOVLW Delay2		    ;moves constant Delay2 into W
    MOVWF Loop_Count		    ;moves contents of W into Loop_Count
    
LOOP_INNER:
    NOP				    ;stalls 1 cycle
    DECF Loop_Count ,F		    ;decrements Loop_Count by F
    BNZ LOOP_INNER		    ;if zero bit=0 then branch back to top of LOOP_INNER, if not, skips this line
    DECF Color_Value,F		    ;decrements Color_Value by F
    BNZ LOOP_OUTER		    ;if zero bit=0 then branch back to top of LOOP_OUTER, if not, skips this lilne
    RETURN
END