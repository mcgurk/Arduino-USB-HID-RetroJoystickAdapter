# Matrix setup for Atari joysticks

matrix
	scanrate 1
	debounce 5
	blocking 1

	# Joystick 1 (Joystick GND to Arduino GND)
	unstrobed -PD1 PAD_8 #pin_2
	unstrobed -PD0 PAD_2 #pin_3
	unstrobed -PD4 PAD_4 #pin_4
	unstrobed -PC6 PAD_6 #pin_5
	unstrobed -PD7 PAD_0 #pin_6

	# Joystick 2 (Joystick GND to Arduino GND)
	unstrobed -PF4 PAD_SLASH #pin_A3
	unstrobed -PF5 PAD_ASTERIX #pin_A2
	unstrobed -PF6 PAD_MINUS #pin_A1
	unstrobed -PF7 PAD_PLUS #pin_A0
	unstrobed -PB1 SCROLL_LOCK #pin_15
end
