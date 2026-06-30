			AREA gcd, CODE
			EXPORT asm_entry  
						
asm_entry                      		
			; Here a is 12 and b is 2, make sure
            ; you try different values when testing.

			mov r0, #21		; a
			mov r1, #2      ; b
			
            ; Place your code here
			
loop		cmp r0, r1		; compare a - b
			beq end_loop	; exit loop when a == b
			
			bgt greater		; if a > b, go to 'greater'
			
			sub r1, r1, r0	; else b = b - a
			b loop			; branch back to loop
			
greater		sub r0, r0, r1	; a = a - b
			b loop			; branch back to loop

end_loop
			b   asm_entry


			END