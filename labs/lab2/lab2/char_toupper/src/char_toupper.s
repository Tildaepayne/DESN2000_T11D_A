			AREA char_touper, CODE
			EXPORT asm_entry  
						
asm_entry                      		
					
			; Write your code here.

			ldrb r0, char

			;if r0 is not lowercase, exit
			cmp r0, #97
			blt exit			; if r0 < 97, exit

			cmp r0, #122
			bgt exit			; if r0 > 122, exit

			sub r0, r0, #32		; subtract 32 from r0 and store result into char
			strb r0, char

exit		b   asm_entry

			AREA character, DATA
char		DCB "f"				; identical to DCB 0x66

			END