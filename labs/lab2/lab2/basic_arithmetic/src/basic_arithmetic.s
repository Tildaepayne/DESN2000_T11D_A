			AREA basic_arith, CODE  ; This compiler directive tells the 
									; compiler that this section is called "basic_arith"
									; and that executable instructions follow.

			EXPORT asm_entry        ; This compiler directive tells the linker that 
									; "main" is in here, tells the linker where to 
									; go after running the startup code.

asm_entry                 			; This is a label, the entry to the main function.

			;;;;;;;;;;;;;;;;;;;;;;;;
	
			ldr r0, a				; load a into r0
			ldr r1, d				; load d into r1
			add r0, r0, r1			; r0 = a + d
			ldr r1, b				; load b into r1
			ldr r2, c				; load c into r2
			add r1, r1, r2			; r1 = b + c
			sub r0, r0, r1			; store result of (a + d) - (b + c) into r0
			
			;;;;;;;;;;;;;;;;;;;;;;;;
                                                                         
			b   asm_entry          	; this is an instruction which sets pc back to main

a   		DCD 32	
b   		DCD 14
c			DCD 1
d			DCD 3

			END