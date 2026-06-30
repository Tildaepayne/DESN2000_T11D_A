			AREA asmcode, CODE
			EXPORT asm_entry  

asm_entry                      		
											
			ldr r0, =a				; The address of "a" is loaded into R0
			ldr r1, [r0]			; The value of "a" is loaded into R1, 
									; using the address obtained in 
									; the previous instruction.
									
									; Now we will store a + b into c:
									
			ldr r3, =b          	; R3 = Address of "b"
			ldr r2, [r3]        	; R2 = 11 (from "b")
			add r0, r1, r2      	; R0 = a + b = 21
			ldr r3, =c          	; R3 = Address of "c"
			str r0, [r3]        	; Store the result to "c"
			b   asm_entry

			AREA varbs, DATA

a			DCD 10              	; Variable "a"
b      		DCD 11              	; Variable "b"
c      		DCD 0               	; Variable "c"
			END
			