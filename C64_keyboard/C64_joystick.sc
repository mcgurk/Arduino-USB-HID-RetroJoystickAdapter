# Matrix setup for Atari joysticks

matrix
	scanrate 1
	debounce 5
	blocking 1

	# Joystick 1
	unstrobed -PD1 PAD_8 #pin2
	unstrobed -PD0 PAD_2 #pin3
	unstrobed -PD4 PAD_4 #pin4
	unstrobed -PC6 PAD_6 #pin5
	unstrobed -PD7 PAD_0 #pin6

	# Joystick 2
	unstrobed -PF4 PAD_SLASH #pinA3
	unstrobed -PF5 PAD_ASTERIX #pinA2
	unstrobed -PF6 PAD_MINUS #pinA1
	unstrobed -PF7 PAD_PLUS #pinA0
	unstrobed -PB1 SCROLL_LOCK #pin15
end