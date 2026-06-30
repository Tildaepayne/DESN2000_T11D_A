			AREA ldr_basic, CODE    ; This assembler directive tells the 
									; assembler that this section is called "ldr_basic"
									; and that executable instructions follow.

			EXPORT asm_entry        ; This assembler directive tells the linker that 
									; "asm_entry" is in here, tells the linker where to 
									; go after running the startup code.

asm_entry                      		; This is a label, the entry program.

			mov r0, #0              ; set r0 to 0
			ldr r1, [pc, #12]       ; load a into r1
			add r0, r0, r1          ; r0 = r0 + r1. r0 now contains "a"
			ldr r2, [pc, #8]        ; load b into r2
			add r0, r0, r2          ; r0 = r0 + r2. r0 now contains "a + b"   
                                                                         
			b   asm_entry          	; this is an instruction which sets pc back to asm_entry

a   		DCD 50					; assembler directive which puts "50" here
b   		DCD 25					; assembler directive which puts "25" here

			END