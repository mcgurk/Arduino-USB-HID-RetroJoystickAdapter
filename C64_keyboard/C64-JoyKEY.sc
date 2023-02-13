# https://github.com/tebl/C64-JoyKEY
# https://github.com/tebl/C64-JoyKEY/blob/main/documentation/schematic/C64%20Joykey.pdf
#
# http://kookye.com/wp-content/uploads/2016/02/Pinout-ProMicro.jpg
# https://deskthority.net/wiki/Arduino_Pro_Micro
#
# PD1(D2) fire1
# PD0(D3) fire2
# PD4(D4) fire3
# PB3(D14) left
# PB1(D15) down
# PF7(A0/D18) right
# PF6(A1/D19) up
# PD7(D6) underglowleds
# PB6(D10) sysled
# PB5(D9) powerled

# '+' prefix = off at startup
# '-' prefix = on at startup
led caps -PD7
led num +PB6
led scroll -PB5

matrix
	scanrate 1
	debounce 5
	blocking 1

	unstrobed -PD1 1
	unstrobed -PD0 2
	unstrobed -PD4 3
	unstrobed -PB3 L
	unstrobed -PB1 D
	unstrobed -PF7 R
	unstrobed -PF6 U
end

# example, reset
macroblock
  macro 3 -ALL
    PRESS LSHIFT
    PRESS LALT
    PRESS LCTRL
    PRESS LGUI
    CLEAR_ALL
  endmacro
endblock

# example, press a-button to CAPS_LOCK = toggle underglowleds
#macroblock
#  macro A -ALL
#    PRESS CAPS_LOCK
#    CLEAR_ALL
#  endmacro
#endblock

# example, press b-button to NUM_LOCK = toggle SYS-led
#macroblock
#  macro B -ALL
#    PRESS NUM_LOCK
#    CLEAR_ALL
#  endmacro
#endblock

# example, press c-button to SCROLL_LOCK = toggle PWR-led
#macroblock
#  macro C -ALL
#    PRESS SCROLL_LOCK
#    CLEAR_ALL
#  endmacro
#endblock

# example, press d-button to get shifted D
#macroblock
#  macro D -ALL
#    SET_META LSHIFT
#    PRESS D
#    CLEAR_ALL
#  endmacro
#endblock

#
# unstrobed
# Specifies a single pin to read and the corresponding HID code.
# This is intended for a single switch which can be wired with the other side of the switch permanently connected to either ground or +5V. If the other side of the switch is at +5V, a pull-down resistor must be added between the pin and ground.
# Example:
# unstrobed -PF1 LSHIFT
