# Matrix setup for COMMODORE 64

#led caps PD0
#led num PD1
#led scroll PF0

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

# arrow left = BACK_QUOTE (section/fraction)
# pound (£) = INSERT
# restore = PAGE_UP
# run/stop = ESC
# commodore = LCTRL
# arrow up = DELETE
# asterisk (*) = RIGHT_BRACE
# minus (-) = EQUAL
# plus (+) = MINUS
# at (@) = LEFT_BRACE
# ctrl = TAB
# lshift = LSHIFT
# rshift = RSHIFT
# equal (=) = BACKSLASH
# colon (:) = SEMICOLON
# semicolon (;) = QUOTE

macroblock
	macro F7 LSHIFT		# LSHIFT + F7 = F12
		PRESS F12
		CLEAR_ALL
	endmacro
endblock

