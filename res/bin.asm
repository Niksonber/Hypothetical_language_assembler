oi: EQU 2
m: macro
load A
endmacro
n: macro &par1
sub &par1
m
endmacro

SECTION TEXT
INPUT OLD_DATA
LOAD OLD_DATA
L1: DIV DOIS
STORE  NEW_DATA
MULT DOIS
STORE TMP_DATA
LOAD OLD_DATA
n
SUB TMP_DATA
STORE TMP_DATA
OUTPUT TMP_DATA
COPY NEW_DATA,OLD_DATA
LOAD OLD_DATA
JMPP L1
STOP
SECTION DATA
DOIS: CONST 2
OLD_DATA: SPACE
NEW_DATA: SPACE
TMP_DATA: SPACE
