			AREA simple_loop, CODE
			EXPORT asm_entry

asm_entry                      
			mov r0, #1			; Initial value of exp is 1
			mov r2, #0			; Initial value of k is 0
for_loop	cmp r2, #31			; Compare k with 31
			bge exit			; Exit loop if k >= 31

			mov r0, r0, lsl #1	; exp = exp * 2 (shifting left by 1 bit is the
								; same as multiplying by 2)			
			add r2, r2, #1		; Increment k

			b for_loop			; Branch back to start of loop

exit		mov pc, lr			; Return result to operating system
			END
