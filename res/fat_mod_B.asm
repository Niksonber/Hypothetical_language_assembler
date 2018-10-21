MOD_B: BEGIN
SECTION TEXT
	FAT: EXTERN
	N: EXTERN
	PUBLIC MOD_B	
	STORE AUX     ; 0, 1
	MULT N        ; 2, 3
	STORE N       ; 4, 5
	LOAD AUX      ; 6, 7
	JMP FAT       ; 8, 9
SECTION DATA
	AUX: SPACE	  ; 10
END
