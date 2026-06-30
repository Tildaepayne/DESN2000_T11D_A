			AREA	array_add, CODE 
			EXPORT 	asm_entry
					
scalar		EQU 4					
					
asm_entry	ldr 	r0, =src	 	; R0 is the address of src
			ldr 	r1, =dst	 	; R1 is the address of dst
			mov		r2, #scalar	 	; Add this to every array element
					
			;;;;;;;;;;;;;;;;;;;;;

			; Your code here   
			; Each entry of dst needs to equal 
			; the corresponding entry of src, plus
			; scalar.
			; Use r3-r10 to temporarily store the array
			
			ldmia r0, {r3-r10}		; load contents of src array into registers r3-r10
			add r3, r3, r2			; add scalar to every element in array
			add r4, r4, r2
			add r5, r5, r2
			add r6, r6, r2
			add r7, r7, r2
			add r8, r8, r2
			add r9, r9, r2
			add r10, r10, r2
			stmia r1, {r3-r10}		; store contents of r3-r10 into dst array
			
			
			;;;;;;;;;;;;;;;;;;;;;
										
			b 		asm_entry
			
					
			AREA	variables, DATA
src			DCD 	3, 4, 5, 7, 13, 54, 92, 1034
dst			DCD 	0, 0, 0, 0, 0, 0, 0, 0
			
			END
					