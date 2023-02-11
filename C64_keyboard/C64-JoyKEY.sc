# https://github.com/tebl/C64-JoyKEY
# https://github.com/tebl/C64-JoyKEY/blob/main/documentation/schematic/C64%20Joykey.pdf
#
# https://deskthority.net/wiki/Arduino_Pro_Micro
#
# PD1(D2) fire1
# PD0(D3) fire2
# PD4(D4) fire3
# PB3(D14) left
# PB1(D15) down
# PF5(D18) right
# PF4(D19) up

matrix
	scanrate 1
	debounce 5
	blocking 1

	sense		PB6		PB2		PB3		PB1		PF7		PF6		PF5		PF4	PD3
	strobe	PD1	1		BACK_QUOTE	TAB		ESC		SPACE		LCTRL		Q		2	UNASSIGNED
	strobe	PD0	3		W		A		LSHIFT		Z		S		E		4	UNASSIGNED
	strobe	PD4	5		R		D		X		C		F		T		6	UNASSIGNED
	strobe	PC6	7		Y		G		V		B		H		U		8	UNASSIGNED
	strobe	PD7	9		I		J		N		M		K		O		0	UNASSIGNED
	strobe	PE6	MINUS		P		L		COMMA		PERIOD		SEMICOLON	LEFT_BRACE	EQUAL	UNASSIGNED
	strobe	PB4	INSERT		RIGHT_BRACE	QUOTE		SLASH		RSHIFT		BACKSLASH	DELETE		HOME	PAGE_UP
	strobe	PB5	BACKSPACE	ENTER		RIGHT		DOWN		F1		F3		F5		F7	UNASSIGNED
end
