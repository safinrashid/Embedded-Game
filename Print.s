sum EQU 0  ;32-bit local variable, stored on the stack
i   EQU 4  ;32-bit local variable, stored on the stack

    IMPORT   ST7735_OutChar
    IMPORT   ST7735_OutString
    EXPORT   LCD_OutDec
    EXPORT   LCD_OutFix

    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB

  
;-----------------------LCD_OutDec-----------------------
; Output a 32-bit number in unsigned decimal format
; Input: R0 (call by value) 32-bit unsigned number
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
; R0=0,    then output "0"
; R0=3,    then output "3"
; R0=89,   then output "89"
; R0=123,  then output "123"
; R0=9999, then output "9999"
; R0=4294967295, then output "4294967295"
LCD_OutDec
	PUSH {LR, R0}

	SUB SP, #8 ;Allocation

	CMP R0, #10
	BLO baseOutDec
	
	MOV R3, #10
	UDIV R2, R0, R3		;R2 = R0/10
	MLS R1, R2, R3, R0	;R1 = R0 - R2*R3 (remainder)
	MOV R0, R2			;new R0
	STR R1, [SP, #sum]	;new input stored in stack
	BL LCD_OutDec		;start over
	
	LDR R0, [SP, #sum]	;last digit load from stack
	ADD R0, #48			;convert to ascii
	BL ST7735_OutChar	;print ascii
	B finish_dec		;output character
	
baseOutDec
	ADD R0, #48
	BL ST7735_OutChar	;print ascii (single digit)
	
finish_dec

	ADD SP, #8 ;Deallocation
		
	POP {LR, R0}
	
      BX  LR
;* * * * * * * * End of LCD_OutDec * * * * * * * *

; -----------------------LCD _OutFix----------------------
; Output characters to LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
; Outputs: none
; E.g., R0=0,    then output "0.000"
;       R0=3,    then output "0.003"
;       R0=89,   then output "0.089"
;       R0=123,  then output "0.123"
;       R0=9999, then output "9.999"
;       R0>9999, then output "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
LCD_OutFix

	MOV R1, #0	;digit counter initialize
	
OutFix

	PUSH {R4, LR}
	
	SUB SP, #8 ;Allocation

	MOV R4, #10000
	CMP R0, R4
	BHS Asterisk	;check to see if too big
	
	CMP R1, #4	;digits/character counter, if 4 digits printed, done
	BEQ finish_fix
	
	MOV R3, #10
	UDIV R2, R0, R3	;R2 = R0/10
	MLS R4, R2, R3, R0 ;R4 = R0 - R2*R3
	MOV R0, R2	;new R0
	STR R4, [SP, #sum]	;new input stored in stack
	STR R1, [SP, #i]	;digit counter stored in stack
	ADD R1, #1			;+1 digit ptint
	BL OutFix
	
	LDR R0, [SP, #sum]	;load digit
	ADD R0, #48			;convert to ascii
	BL ST7735_OutChar	;print
	
	LDR R1, [SP, #i]	;load content from stack
	CMP R1, #3			;decimal counter
	BNE finish_fix	;leave
	
	MOV R0, #46			;ascii for period(.)
	BL ST7735_OutChar
	B finish_fix	;leave
	
Asterisk

	MOV R0, #42
	BL ST7735_OutChar	;(*).***
	MOV R0, #46
	BL ST7735_OutChar	;*(.)***
	MOV R0, #42
	BL ST7735_OutChar	;*.(*)**
	MOV R0, #42
	BL ST7735_OutChar	;*.*(*)*
	MOV R0, #42
	BL ST7735_OutChar	;*.**(*)
	
finish_fix

	ADD SP, #8 ;Deallocation
	
	
	POP {R4, LR}


     BX   LR
 
     ALIGN
;* * * * * * * * End of LCD_OutFix * * * * * * * *

     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
