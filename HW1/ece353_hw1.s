; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export hw1
    export TRANSACTIONS_TOTAL
    export TRANSACTIONS
    export TEAM
    export PERSON1
    export PERSON2
	
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
; ALLOCATE SRAM HERE
TRANSACTIONS_TOTAL  SPACE	4		;Need 7*4 Bits so allocating 4 Bytes
TRANSACTIONS		SPACE	400		;Each Trans is 4 bytes long, need 100

    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
TEAM        DCB     "Team 18"
            DCB     0
PERSON1     DCB     "Ryan Bambrough"
            DCB     0
PERSON2     DCB     "Nicholas Adams"
            DCB     0
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
hw1   PROC
    
    PUSH {R0-R12, LR}
    ; DO NOT MODIFY ANTHING ABOVE THIS LINE!!!
    ; DO NOT MODIFY REGISTERS R13-R15!!!

	;*******************************************;
	; Definitions								;
	; R0 = Sub-Total (Current Total)			;
	; R1 = Pre-Total (All Total)				;
	; R2 = 1,000,000s							;
	; R3 = 100,000s								;
	; R4 = 10,000s								;
	; R5 = 1,000s								;
	; R6 = 100s									;
	; R7 = 10s									;
	; R8 = 1s									;
	; R9 - R10 = Temporary Registers			;
	; R11 = Record of place in TRANSACTIONS		;
	; R12 = Undecoded 32-bit Trans-Code			;
	;*******************************************;
	
	; Decoding Format
	; 	Filter BCD into temp Reg
	; 	Convert BCD to Number, storing into Reg
	
	BIC R0, R0, R0		; Clear Total
	BIC R1, R1, R1
	BIC R2, R2, R2
	BIC R3, R3, R3
	BIC R4, R4, R4
	BIC R5, R5, R5
	BIC R6, R6, R6
	BIC R7, R7, R7
	BIC R8, R8, R8
	BIC R9, R9, R9
	BIC R10, R10, R10
	BIC R11, R11, R11 	; Clear Pointer
	BIC R12, R12, R12

	LDR R11, =(TRANSACTIONS)
	
	SUB R11, R11, #4

	;Error Handling
Error
	
	ADD R11, R11, #4
	
	; Load Trans-code
Start
	LDR R12, [R11]
	
	; Test if Finished
	
	LSR R9, R12, #28
	CMP R9, #0x0F
	BEQ FIN
	
	;*******************;
	; Split BCD Digits
	;*******************;
	
	AND R9, R12, #0x0F000000	;Millions
	LSR R2, R9, #24				;BCD # now in 0x0?
	CMP R2, #9					;Check for Valid BCD number
	BGT Error
	
	AND R9, R12, #0x00F00000	;Hundred Thou
	LSR R3, R9, #20
	CMP R3, #9
	BGT Error
	
	AND R9, R12, #0x000F0000	;Ten Thou
	LSR R4, R9, #16
	CMP R4, #9
	BGT Error
	
	AND R9, R12, #0x0000F000	;Thou
	LSR R5, R9, #12
	CMP R5, #9
	BGT Error
	
	AND R9, R12, #0x00000F00	;Hundred
	LSR R6, R9, #8
	CMP R6, #9
	BGT Error
	
	AND R9, R12, #0x000000F0	;Ten
	LSR R7, R9, #4
	CMP R7, #9
	BGT Error
	
	AND R8, R12, #0x0000000F	;Ones
	CMP R8, #9
	BGT Error

	;Sum Into Number
		;R9 = Mutiply Track Reg
		;R10 = Multiply Reg
		
	MOV R9, #1		;1
	MOV R10, #10
	MOV R0, #0		;Zero R0 for Sub-total
	
	MUL R8, R8, R9
	ADD R0, R8, R0
	
	MUL R9, R9, R10	;10
	
	MUL R7, R7, R9
	ADD R0, R7, R0
	
	MUL R9, R9, R10	;100
	
	MUL R6, R6, R9
	ADD R0, R6, R0

	MUL R9, R9, R10	;1,000
	
	MUL R5, R5, R9
	ADD R0, R5, R0
	
	MUL R9, R9, R10	;10,000
	
	MUL R4, R4, R9
	ADD R0, R4, R0
	
	MUL R9, R9, R10 ;100,000
	
	MUL R3, R3, R9
	ADD R0, R3, R0
	
	MUL R9, R9, R10	;1,000,000
	
	MUL R2, R2, R9
	ADD R0, R2, R0

	;Total now in R0

	; Determine to Add or Subtract
	LSR R9, R12, #28
	
	CMP R9, #0x0A		;Add if A
	ADDEQ R1, R0, R1
	
	CMP R9, #0x0B		;Subtract if B
	SUBEQ R1, R1, R0
	
	ADD R11, R11, #4
	
	B Start				;Repeat Until Finish Condition
	
	;Save Total with Encoding
	
	;R12 = Backup Binary Number
	;R0 = For loop counter
	;R1 = Destructable Binary Number
	;R2 - R8 = 1,000,000's through 1's
FIN
	BIC R2, R2, R2		;Clear Regs
	BIC R3, R3, R3
	BIC R4, R4, R4
	BIC R5, R5, R5
	BIC R6, R6, R6
	BIC R7, R7, R7
	BIC R8, R8, R8
	BIC R10, R10, R10
	
	MOV R0, #0
	
	MOV R12, R1			;Copy binary number into R12
	
	;Check if Number is negative if 2's complement
	
	CMP R1, #0
	MVNLT R1, R1
	ADDLT R1, R1, #1
	
For
		;Algo = Checking if each place is greater than or equal to 5, if then add 3

	CMP R0, #28
	BEQ Finalize
	
	CMP R8, #5
	ADDGE R8, R8, #3
	
	CMP R7, #5
	ADDGE R7, R7, #3
	
	CMP R6, #5
	ADDGE R6, R6, #3
	
	CMP R5, #5
	ADDGE R5, R5, #3
	
	CMP R4, #5
	ADDGE R4, R4, #3
	
	CMP R3, #5
	ADDGE R3, R3, #3
	
	CMP R2, #5
	ADDGE R2, R2, #3
	
	;Shifting all bits left by 1 masking MSB of all into next reg
	
	LSL R2, R2, #1			;1,000,000
	AND R9, R3, #0x08
	ORR R2, R2, R9, LSR #3
	AND R2, R2, #0xF
	
	LSL R3, R3, #1			;100,000
	AND R9, R4, #0x08
	ORR R3, R3, R9, LSR #3
	AND R3, R3, #0xF
	
	LSL R4, R4, #1			;10,000
	AND R9, R5, #0x08
	ORR R4, R4, R9, LSR #3
	AND R4, R4, #0xF
	
	LSL R5, R5, #1			;1,000
	AND R9, R6, #0x08
	ORR R5, R5, R9, LSR #3
	AND R5, R5, #0xF
	
	LSL R6, R6, #1			;100
	AND R9, R7, #0x08
	ORR R6, R6, R9, LSR #3
	AND R6, R6, #0xF
	
	LSL R7, R7, #1			;10
	AND R9, R8, #0x08
	ORR R7, R7, R9, LSR #3
	AND R7, R7, #0xF
	
	LSL R8, R8, #1			;1
	AND R8, R8, #0xF
	AND R9, R1, #0x8000000
	LSL R1, R1, #1
	ORR R8, R8, R9, LSR #27
	
	ADD R0, R0, #1
	
	B For
	
Finalize
	CMP R12, #0						;Check if binary is less than zero
	MOVGE R10, #0xA0000000			;Tack on A if postive or Zero
	MOVLT R10, #0xB0000000			;Tack on B if negative
	
	ORR R10, R10, R2, LSL #24
	ORR R10, R10, R3, LSL #20
	ORR R10, R10, R4, LSL #16
	ORR R10, R10, R5, LSL #12
	ORR R10, R10, R6, LSL #8
	ORR R10, R10, R7, LSL #4
	ORR R10, R10, R8
	
	LDR R11, =(TRANSACTIONS_TOTAL)	;Store Value
	STR R10, [R11]
	
    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!        
RETURN
    POP {R0-R12, PC}
    
    ENDP
    align
        

    END
        
        