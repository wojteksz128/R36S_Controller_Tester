#!/bin/bash

# Point SDL2 library to the system gamepad database
export SDL_GAMECONTROLLERCONFIG_FILE="/opt/inttools/gamecontrollerdb.txt"
export SDL_JOYSTICK_DISABLE_HAT=0

# Navigate to the program directory, grant execution permissions, and run
cd /roms/ports/PadTester
chmod +x PadTester
./PadTester