			AREA	array_add, CODE 
			EXPORT 	asm_entry			
					
asm_entry	

			mov r0, #1
			add pc, pc, #28
			mov r1, #1
			add pc, pc, #4
			mov r2, #1
			add pc, pc, #20
			mov r3, #1
			sub pc, pc, #20
			mov r4, #1
			add pc, pc, #20
			mov r5, #1
			sub pc, pc, #20
			mov r6, #1
			sub pc, pc, #4
			mov r7, #1
			add pc, pc, #4
			mov r8, #1
			sub pc, pc, #68
			
			b asm_entry
			
			END
					