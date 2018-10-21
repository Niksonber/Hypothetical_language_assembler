MOD_B: BEGIN
SECTION TEXT
	FAT: EXTERN
	N: EXTERN
	PUBLIC MOD_B	
	STORE AUX+1   ; 0, 1 (testing array access)
	MULT N        ; 2, 3
	STORE N       ; 4, 5
	LOAD AUX+1    ; 6, 7 (testing array access)
	JMP FAT       ; 8, 9
SECTION DATA
	AUX: SPACE 2  ; 10   (testing array access)
END
