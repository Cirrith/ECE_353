; Filename:     ece353_hw2.s
; Author:       ece353 staff 
; Description:  

    export TEAM
    export PERSON1
    export PERSON2
    export str_emphasize
		
    export str_length
		
    export str_copy
    export str_compare
    export str_to_upper
    export str_fill
		
	export str_icompare
        
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
TEAM        DCB     "Team18"
            DCB     0
PERSON1     DCB     "Ryan Bambrough"
            DCB     0
PERSON2     DCB     "Nicholas Adams"
            DCB     0
    align

;**********************************************
; Code (FLASH) Segment
;**********************************************


;*****************************************************************************
; str_length
;
; Description: Returns the length of a null terminated string.  
;
; Parameters
;    R0                : The address of the string being examined
;
; Return values:			  Description
;	R15
;  -1                         : NULL pointer passed in
;  -2                         : max. string length exceeded
;  string length              : String length (max 1024)
; 
; C Prototype
;     int32_t str_length(char *string)
;*****************************************************************************

;R0 = I/O
;R1 = Counter
;R2 = Cur Value

str_length   PROC
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_l
	MOV R1, #0			
While_l					; While Loaded Value != 0
	LDRB R2, [R0, R1]
	ADD R1, R1, #1		; Update total length

	CMP R1, #1024		; Return -2 if > 1024
	MOVGT R0, #-2
	BEQ Ret_l
	
	CMP R2, #0 			; At end of string?
	MOVEQ R0, R1		; Move length to return reg
	BEQ Ret_l
	B While_l

Ret_l
    BX  LR
    ENDP
        
;**********************************************
; str_copy
;
; Description: Copies a string from the src address to the dest.  
;
;    Parameters
;   	R0               	: The address where the string will be copied from
;   	R1               	: The address where the string will be copied to
;   	R2               	: length of the string (max 1024)
;
; Return values:			  	Description
;	R0
;  -1                       	: Invalid address for src or dest
;  -2                       	: max. string length exceeded
;   0                       	: Success
;
; C Prototype
;     int32_t str_copy(char *src, char *dest, uint16_t size)
;**********************************************

;R0 = From
;R1 = To
;R2 = Remaining
;R3 = Temporary Storage
	
str_copy   PROC	
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_c
	
	CMP R1, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_c
	
	CMP R2, #1024 		; Check for max length
	MOVGT R0, #-2
	BGT Ret_c

For_c					; For <length, length --
	LDRB R3, [R0, R2] 	; Moving backwards
	STRB R3, [R1, R2]
	
	SUB R2, R2, #1
	CMP R2, #0			; Check if at start of string
	BGE For_c
	MOV R0, #0

Ret_c
    BX  LR
    ENDP
        
;*****************************************************************************
; str_compare
;
; Description: Compares to see if two strings are identical.  
;
; Parameters
;   R0           		: The address of the 1st string
;   R1          		: The address of the 2nd string
;   R2           		: length of the string (max 1024)
;
; Return values:			  Description
;  -1                         	: Invalid address for src or dest
;  -2                         	: max. string length exceeded
;   0                         	: match
;   1                         	: does not match
;
; C Prototype
;    int32_t str_compare(char *str_A, char *str_B, uint16_t size)
;*****************************************************************************

;R0 = With
;R1 = What
;R2 = Length
;R3 = Stor1
;R4 = Stor2
	
str_compare   PROC
	PUSH {R4}
	
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_cmp
	
	CMP R1, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_cmp
	
	CMP R2, #1024 		; Check for max length
	MOVGT R0, #-2
	BGT Ret_cmp
	
	SUB R2, R2, #1
For_cmp
	LDRB R3, [R0, R2]	; Load characters
	LDRB R4, [R1, R2]
	
	SUB R2, R2, #1		; decrement size
	CMP R3, R4			; Return 1 if !=
	BNE Fin_cmp
	
	CMP R2, #0			; Return match
	MOVEQ R0, #0
	BEQ Ret_cmp
	B	For_cmp
Fin_cmp
	MOV R0, #1			; Return not match

Ret_cmp
	POP {R4}
    BX  LR
    ENDP
		
;R0 = With
;R1 = What
;R2 = Length
;R3 = Stor1
;R4 = Stor2
;R5 = tmp
	
str_icompare   PROC
	PUSH {R4}
	
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_icmp
	
	CMP R1, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_icmp
	
	CMP R2, #1024 		; Check for max length
	MOVGT R0, #-2
	BGT Ret_icmp
	
	SUB R2, R2, #1
For_icmp
	LDRB R3, [R0, R2]	; Load characters
	LDRB R4, [R1, R2]
	SUB R2, R2, #1		; decrement size
	
	CMP R3, R4			; 1) Equal
	BEQ equal
	
	CMP R3, #65		; this is a letter?
	BLT Fin_icmp
	CMP R3, #122
	BGT Fin_icmp
	
	CMP R3, #90
	BLE next_char
	CMP R3, #97
	BGE next_char
	B Fin_icmp
	
next_char
	CMP R4, #65		; this is a letter?
	BLT Fin_icmp
	CMP R4, #122
	BGT Fin_icmp
	
	CMP R4, #90
	BLE test_char
	CMP R4, #97
	BGE test_char
	B Fin_icmp

test_char
	ADD	R4, R4, #32
	CMP R3, R4
	BEQ equal
	
	SUB	R4, R4, #64
	CMP R3, R4
	BEQ equal
	
	B Fin_icmp
	
equal
	CMP R2, #0			; Return match
	MOVEQ R0, #0
	BEQ Ret_icmp
	B	For_icmp
Fin_icmp
	MOV R0, #1			; Return not match

Ret_icmp
	POP {R4}
    BX  LR
    ENDP
        
;*****************************************************************************
; str_to_upper
;
; Description: All alphabetic characters are converted to upper case.
;
; Parameters
;  1st Param                    : Address of string being converted to upper case
;  2nd Param                    : length of the string (max 1024)
;
; Return values:				Description
;  -1                           : Invalid address for src or dest
;  -2                           : max. string length exceeded
;   0                           : success
; C Prototype
; int32_t str_to_upper(char *str, uint16_t size)
;*****************************************************************************

; R0 String
; R1 Length
; R2 character

str_to_upper   PROC	
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_up
	
	CMP R1, #1024 		; Check for max length
	MOVGT R0, #-2
	BGT Ret_up
	
	SUB R1, R1, #1		; Don't touch \0
For_up
	LDRB R2, [R0, R1]	; Load character
	SUB R1, R1, #1
	
	CMP R1, #-1			; Return 0 if at start of string
	MOVLT R0, #0
	BLT Ret_up
	
	CMP R2, #97			; Is this a lowercase letter?
	BLT For_up
	CMP R2, #122
	BGT For_up
	
	SUB R2, R2, #32
	ADD R1, R1, #1
	STRB R2, [R0, R1]
	SUB R1, R1, #1
	B For_up
	
Ret_up
    BX  LR
    ENDP

;*****************************************************************************
; str_fill
;
; Description: Fills an entire string with a given value
;
; Parameters
;  1st Param                : The address of the string 
;  2nd Param                : value to write into the string
;  3rd Param                : length of the string (max of 1024)
;
; Return values:             Description
;  -1                        : Invalid address for src or dest
;  -2                        : max. string length exceeded
;   0                        : success
;
; C Prototpye
;    int32_t str_fill(char *str, char value, uint16_t size)
;*****************************************************************************
; R0 String
; R1 character
; R2 Length

str_fill   PROC	
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_f

	CMP R2, #1024 		; Check for max length
	MOVGT R0, #-2
	BGT Ret_f
	
	SUB R2, R2, #1		; Don't touch \0
For_f
	SUB R2, R2, #1
	
	CMP R2, #0			; Return 0 if at start of string
	MOVLT R0, #0
	BLT Ret_f
	
	STRB R1, [R0, R2]
	B For_f
	
Ret_f
    BX  LR
    ENDP
        
;*****************************************************************************
; str_emphasize
;
; Description: Performs a case insensitive search for a substring in text.  
; Any substrings that will be found will be converted to UPPERCASE.  The 
; function will return how many occurrances of the substring were found
;
;    Parameters
;    1st Param           : The address of the text string that will be searched.  
;                        : The maximum length of this string is 1024 characters
;
;	 2nd Param           : The address of the sub-string in 'text' that will be 
;                        : emphasized.  The maximum length of the search text is
;                        : 1024 characters.
;
; Return values:
;  -1                        : Invalid address for src or dest
;  -2                        : max. string length exceeded
;                            : The function will return how many occurrances of the 
;                            : substring were found
; C Prototype
;    int32_t str_emphasize(char *text, char *search)
;*****************************************************************************
; R0 - string to be searched
; R1 - search term
; R2 - length of term
; R3 - length of string
; R4 - index of search string current char
; R5 - R0 base
; R6 - current char of search string
; R7 - current char of search search term
; R8 - Counter
; R9 - tmp
; R10 - tmp
str_emphasize   PROC
	PUSH {R4-R10, R14}
	CMP R0, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_emp
	
	CMP R1, #0			; Tests if NULL ptr
	MOVEQ R0, #-1
	BEQ Ret_emp
	
	MOV R10, R0	; Test if search string is too long
	PUSH {R1-R3}
	BL str_length
	POP {R1-R3}
	CMP R0, #0
	BLT	Ret_emp
	MOV R3, R0
	
	MOV R0, R1	; Test if search term is too long
	PUSH {R1-R3}
	BL str_length
	POP {R1-R3}
	CMP R0, #0
	BLT	Ret_emp
	MOV R2, R0
	
	MOV R0, R10			; Return R0
	
	CMP R2, R3			; Give up if term > string
	MOVGT R0, #0
	BGT Ret_emp
	
	SUB R2, R2, #1		; Don't compare \0
	SUB R3, R3, #1		; Don't compare \0
	
	MOV R4, #0
	MOV R5, R0
	MOV R8, #0
	
	; LOOP
	; Chuckin' Method
For_emp
	SUB R9, R3, R2
	SUB R9, R9, R4
	CMP R9, #0
	BLT Fin_emp
	
	ADD R0, R5, R4
	MOV R9, R2
	PUSH {R1-R3}
	BL str_icompare
	POP {R1-R3}
	MOV R2, R9
	
	CMP R0, #1
	ADDEQ R4, R4, #1
	BEQ For_emp
	
	ADD R8, R8, #1
	
	ADD R0, R5, R4
	MOV R9, R1
	MOV R1, R2
	PUSH {R1-R3}
	BL str_to_upper
	POP {R1-R3}
	MOV R1, R9
	
	ADD R4, R4, #1
	B For_emp
Fin_emp
	MOV R0, R8
	
Ret_emp
	POP {R4-R10, R14}
    BX  LR
    ENDP
         
    align
       
    END
        
        