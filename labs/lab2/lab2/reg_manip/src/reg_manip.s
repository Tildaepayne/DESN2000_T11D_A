			AREA reg_manip, CODE    
			EXPORT asm_entry        

scalar		EQU 3					; The compiler directive EQU defines the symbol
									; "scalar" to have the value 3, you can think of this
									; as the same definition as a #define in c.

asm_entry                      		


			;;;;;;;;;;;;;;;;;;;;;;;;
	
			; Your code goes here.
			sub r0, r0, r0
			add r0, r0, #scalar		; store scalar into r0
			mov r1, r0, lsl #1		; in r1, compute scalar * 2^1
			mov r2, r0, lsl #2		; in r2, compute scalar * 2^2
			mov r3, r0, lsl #3		; etc.
			mov r4, r0, lsl #4
			mov r5, r0, lsl #5
			mov r6, r0, lsl #6
			mov r7, r0, lsl #7
			
			
			;;;;;;;;;;;;;;;;;;;;;;;;
                                                                         
			b   asm_entry          	
			END