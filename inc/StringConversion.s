; StringConversion.s
; Student names: Arnav Surjan
; Last modification date: 3/6/2023
; Runs on TM4C123
; number to string conversions

 
    EXPORT   Dec2String
    EXPORT   Fix2String
    AREA    |.text|, CODE, READONLY, ALIGN=2
    THUMB
    PRESERVE8

DecIdx EQU 0
;-----------------------Dec2String-----------------------
; Convert a 32-bit number into unsigned decimal format
; String the string into the empty array add null-termination
; Input: R0 (call by value) 32-bit unsigned number
;        R1 pointer to empty array
; Output: none
; Invariables: This function must not permanently modify registers R4 to R11
Dec2String
	PUSH {R4-R11}
	SUB SP, #4
	MOV R11, SP
	MOV R6, #10
	MOV R7, #0
	STR R7, [R11, #DecIdx]
	MOV R5, #0x30
	
	CMP R0, #0
	BNE DecAppend
	LDR R10, [R11, #DecIdx]
	STRB R5, [R1, R10]
	ADD R10, #1
	STR R10, [R11, #DecIdx]
	B DecNullTerm
	
DecAppend
	CMP R0, #0
	BEQ DecReverse
	LDR R10, [R11, #DecIdx]
	MOV R4, R0
	UDIV R0, R6
	MUL R0, R6
	SUB R5, R4, R0
	ADD R5, #0x30
	STRB R5, [R1, R10]
	ADD R10, #1
	STR R10, [R11, #DecIdx]
	UDIV R0, R6
	B DecAppend
	
DecReverse             
    LDR R10, [R11, #DecIdx]
	MOV R9, R10 
	SUB R9, #1
	MOV R2, #0              
DecRevLoop
    CMP R2, R9              
    BGE DecNullTerm          
    LDRB R3, [R1, R2]        
    LDRB R4, [R1, R9]        
    STRB R4, [R1, R2]        
    STRB R3, [R1, R9]        
    ADD R2, R2, #1          
    SUB R9, R9, #1          
    B DecRevLoop
	
DecNullTerm
	LDR R10, [R11, #DecIdx]
	STRB R7, [R1, R10]
	B DecDone
	
DecDone
	ADD SP, #4
	POP {R4-R11}
    BX LR
;* * * * * * * * End of Dec2String * * * * * * * *

FixDP EQU 0
FixSPACE EQU 4
FixSTAR EQU 8
FixNULL EQU 12
FixIdx EQU 16
; -----------------------Fix2String----------------------
; Create characters for LCD display in fixed-point format
; unsigned decimal, resolution 0.001, range 0.000 to 9.999
; Inputs:  R0 is an unsigned 32-bit number
;          R1 pointer to empty array
; Outputs: none
; E.g., R0=0,    then create "0.000 "
;       R0=3,    then create "0.003 "
;       R0=89,   then create "0.089 "
;       R0=123,  then create "0.123 "
;       R0=9999, then create "9.999 "
;       R0>9999, then create "*.***"
; Invariables: This function must not permanently modify registers R4 to R11
Fix2String
	PUSH {R4-R11}
	SUB SP, #20
	MOV R11, SP
	
	MOV R2, #0x2E ; register/stack setup
	STRB R2, [R11, #FixDP]
	MOV R2, #0x20
	STRB R2, [R11, #FixSPACE]
	MOV R2, #0x2A
	STRB R2, [R11, #FixSTAR]
	MOV R2, #0x00
	STRB R2, [R11, #FixNULL]
	MOV R2, #0
	STR R2, [R11, #FixIdx]
	MOV R6, #10
	
	LDR R8, =9999
	CMP R0, R8
	BHI FixAsterisks

FixAppend
	LDR R10, [R11, #FixIdx]
	CMP R10, #5
	BEQ FixReverse
	MOV R4, R0
	UDIV R0, R6
	MUL R0, R6
	SUB R5, R4, R0
	ADD R5, #0x30
	STRB R5, [R1, R10]
	UDIV R0, R6
	ADD R10, #1
	STR R10, [R11, #FixIdx]
	CMP R10, #3
	BNE FixAppend
	LDRB R7, [R11, #FixDP]
	STRB R7, [R1, R10]
	ADD R10, #1
	STR R10, [R11, #FixIdx]
	B FixAppend
	
FixReverse
	LDR R10, [R11, #FixIdx]
	MOV R9, R10 
	SUB R9, #1
	MOV R2, #0              
FixRevLoop
    CMP R2, R9              
    BGE FixAddSpace     
    LDRB R3, [R1, R2]        
    LDRB R4, [R1, R9]        
    STRB R4, [R1, R2]        
    STRB R3, [R1, R9]        
    ADD R2, R2, #1
    SUB R9, R9, #1
    B FixRevLoop
	
FixAddSpace
	LDR R10, [R11, #FixIdx]
	LDRB R7, [R11, #FixSPACE]
	STRB R7, [R1, R10]
	ADD R10, #1
	STR R10, [R11, #FixIdx]
	B FixNullTerm
	
FixAsterisks
	LDR R10, [R11, #FixIdx]
	LDRB R7, [R11, #FixSTAR]
	STRB R7, [R1, R10]
	ADD R10, #1
	LDRB R7, [R11, #FixDP]
	STRB R7, [R1, R10]
	ADD R10, #1
	LDRB R7, [R11, #FixSTAR]
	STRB R7, [R1, R10]
	ADD R10, #1
	LDRB R7, [R11, #FixSTAR]
	STRB R7, [R1, R10]
	ADD R10, #1
	LDRB R7, [R11, #FixSTAR]
	STRB R7, [R1, R10]
	ADD R10, #1
	STR R10, [R11, #FixIdx]
	B FixNullTerm
	
FixNullTerm
	LDR R10, [R11, #FixIdx]
	LDR R7, [R11, #FixNULL]
	STRB R7, [R1, R10]
	
	
FixDone
	ADD SP, #20
	POP {R4-R11}
    BX LR



 


     ALIGN                           ; make sure the end of this section is aligned
     END                             ; end of file
