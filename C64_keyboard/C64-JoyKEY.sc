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

	unstrobed -PD1 A
	unstrobed -PD0 B
	unstrobed -PD4 C
	unstrobed -PB3 D
	unstrobed -PB1 E
	unstrobed -PF5 F
	unstrobed -PF4 G

end

#
# unstrobed
# Specifies a single pin to read and the corresponding HID code.
# This is intended for a single switch which can be wired with the other side of the switch permanently connected to either ground or +5V. If the other side of the switch is at +5V, a pull-down resistor must be added between the pin and ground.
# Example:
# unstrobed -PF1 LSHIFT
