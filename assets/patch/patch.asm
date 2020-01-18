rLCD EQU $FF40
rSTAT EQU $FF41
SECTION	"Org $72",HOME[$72]
	CALL	THREE
	POP	HL
	INC	HL
	JR	$6A
	
	POP	DE
	POP	BC
	jr continue
waitVRAM:
    ldh a, [rLCD]
    and %10000000
	jr z, continue
    jr z, waitVRAM
    ldh a, [rSTAT]
    and 2
    jr z, waitVRAM
waitVRAMBeginning:
    ldh a, [rLCD]
    and %10000000
	jr z, continue
    ldh a, [rSTAT]
    and 2
    jr nz, waitVRAMBeginning
continue:
	POP	AF
	POP	HL
	RETI

THREE:
	JP	HL
	