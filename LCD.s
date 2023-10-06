GPIO_PORTA_DATA_R       EQU   0x400043FC
SSI0_DR_R               EQU   0x40008008
SSI0_SR_R               EQU   0x4000800C

      EXPORT   writecommand
      EXPORT   writedata

      AREA    |.text|, CODE, READONLY, ALIGN=2
      THUMB
      ALIGN

writecommand
;; --UUU-- Code to write a command to the LCD
;1) Read SSI0_SR_R and check bit 4, 
;2) If bit 4 is high, loop back to step 1 (wait for BUSY bit to be low)
;3) Clear D/C=PA6 to zero
;4) Write the command to SSI0_DR_R
;5) Read SSI0_SR_R and check bit 4, 
;6) If bit 4 is high, loop back to step 5 (wait for BUSY bit to be low)

CommandHigh1
    LDR R1, =SSI0_SR_R ;read SR
	LDR R1, [R1]
	AND R1, R1, #0x10
	LSR R1, R1, #4		;isolate bit 4
	CMP R1, #1			;check bit 4
	BEQ CommandHigh1	;wait for 0
	
	LDR R1, =GPIO_PORTA_DATA_R
	LDR R2, [R1]
	BIC R2, #0x40		;clear DC pa6
	STR R2, [R1]
	
	LDR R1, =SSI0_DR_R
	STR R0, [R1]		;write command
	
CommandHigh2
	LDR R1, =SSI0_SR_R	;read SR
	LDR R1, [R1]
	AND R1, R1, #0x10
	LSR R1, R1, #4		;isolate bit 4
	CMP R1, #1			;check bit 4
    BEQ CommandHigh2	;wait for 0
	
    BX  LR                          ;   return
    

; This is a helper function that sends an 8-bit data to the LCD.
; Input: R0  8-bit data to transmit
; Output: none
; Assumes: SSI0 and port A have already been initialized and enabled
writedata
;; --UUU-- Code to write data to the LCD
;1) Read SSI0_SR_R and check bit 1, 
;2) If bit 1 is low loop back to step 1 (wait for TNF bit to be high)
;3) Set D/C=PA6 to one
;4) Write the 8-bit data to SSI0_DR_R

FIFOLow
    LDR R1, =SSI0_SR_R 
	LDR R1, [R1]
	AND R1, R1, #0x02
	LSR R1, R1, #1
	CMP R1, #0
	BEQ FIFOLow		;check bit 1 loop
	
	LDR R1, =GPIO_PORTA_DATA_R
	LDR R2, [R1]
	ORR R2, #0x40		;set DC pa6
	STR R2, [R1]
	
	LDR R1, =SSI0_DR_R
	STR R0, [R1]		;write command
    
    BX  LR                          ;   return


    ALIGN                           ; make sure the end of this section is aligned
    END                             ; end of file
