			AREA signed_cmp, CODE
			EXPORT asm_entry  
						
asm_entry                      		
					
			ldr r0, =a		; address of a
			ldr r0, [r0]	; r0 contains value at a
			
			ldr r1, =b		; address of b
			ldr r1, [r1]	; r1 contains value at b
			
			cmp r0, r1		; compare a and b, "a - b"
			
			; add conditional fields to
			; the mov instructions below 
			; so they execute only if...
			
			movhi r2, #1		; a > b (unsigned)
			
			movgt r3, #1		; a > b (signed)
			
			movle r4, #1		; a - b is "negative" or zero
			
			movlt r5, #1		; N flag is not equal to the V flag

			b   asm_entry

			AREA integers, DATA
a			DCD 0xE0001234
b			DCD 0x00004567

			END