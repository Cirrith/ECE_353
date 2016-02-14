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
;	R0
;  -1                         : NULL pointer passed in
;  -2                         : max. string length exceeded
;  string length              : String length (max 1024)
; 
; C Prototype
;     int32_t str_length(char *string)
;*****************************************************************************
	/*
	R0 = I/O
	R1 = Counter
	R2 = Cur Value
	*/
str_length   PROC
    PUSH {R1,R2}
	MOV R1, #0
While_l					;While Loaded Value != 0
	LDR R2, [R0, R1]
	CMP R2, #0
	ADDGT R1, R1, #1
	BEQ While_1
Fin_l
	MOV R0, R1
	POP {R1,R2}
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
	/*
	R0 = From
	R1 = To
	R2 = Remaining 
	*/
str_copy   PROC
	PUSH {}

Fin_c
	
    BX  LR
    ENDP
        
;*****************************************************************************
; str_compare
;
; Description: Compares to see if two strings are identical.  
;
; Parameters
;   1st Param           		: The address of the 1st string
;   2nd Param           		: The address of the 2nd string
;   3rd Param           		: length of the string (max 1024)
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
str_compare   PROC
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
str_to_upper   PROC
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
str_fill   PROC
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
str_emphasize   PROC
    BX  LR
    ENDP
        
        
        
    align
        

    END
        
        