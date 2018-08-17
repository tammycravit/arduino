##############################################################################
# Arduino Makefile for the LED Chaser project
#
# Relies on the sketch name and directory name being the same, and the
# Arduino tools being installed in /usr/share/arduino.
##############################################################################

#=============================================================================
# Toolkit locations
#=============================================================================

ARDUINO_DIR   = /usr/share/arduino
ARDMK_TOOLS   = /usr/share/arduino
AVR_TOOLS_DIR = /usr

USER_LIB_PATH := $(realpath ../Libraries)

#=============================================================================
# Extra Arduino libraries to include. These libraries should be installed in
# /usr/share/arduino/libraries, or in the Libraries folder in the repo.
#=============================================================================

ARDUINO_LIBS   = Bounce2

#=============================================================================
# Target board - use "make show_boards" to list valid options.
#=============================================================================

BOARD_TAG      = nano328

#=============================================================================
# Serial monitor port
#=============================================================================

MONITOR_PORT   = /dev/ttyUSB0

#=============================================================================
# And include the Arduino makefile. This is where all the magic happens.
#=============================================================================

include /usr/share/arduino/Arduino.mk
