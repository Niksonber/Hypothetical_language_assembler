MOD_A: BEGIN
SECTION TEXT
	MOD_B: EXTERN
	PUBLIC FAT
	PUBLIC N
	INPUT N         ; 0, 1
	LOAD N          ; 2, 3
	FAT: SUB ONE    ; 4, 5		
	JMPZ FIM        ; 6, 7	
	JMP MOD_B       ; 8, 9	
	FIM: OUTPUT N   ; 10, 11		
	STOP            ; 12
SECTION DATA		
	N: SPACE        ; 13	
	ONE: CONST      ; 14, 15 (testing hexes)
END