			AREA string_to_int, CODE
			EXPORT asm_entry  
						
asm_entry                      		
	
            ; Place your code here

			b   asm_entry

            AREA stringData, DATA
			; Here are the characters of the string
thestring   DCB "12345"
			DCB 0x00 	;explicit null termination
			
			END


