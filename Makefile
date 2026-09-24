# Use Bash to handle secure password input (read -s)
SHELL := /bin/bash

# Optional cross-compiler prefix
CROSS_COMPILE ?= 

CXX = $(CROSS_COMPILE)g++

# Directory structure
SRC_DIR = src
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SDL_INC_DIR = $(BUILD_DIR)/local_sdl2

# Double -I flag resolves the issue with nested internal SDL2 includes
ifeq ($(CROSS_COMPILE),aarch64-linux-gnu-)
	SDL_CFLAGS = -I$(SDL_INC_DIR) -I$(SDL_INC_DIR)/SDL2 -D_REENTRANT
	SDL_LDFLAGS = -L/usr/lib/aarch64-linux-gnu -lSDL2 -lSDL2_ttf
else
	SDL_CFLAGS = $(shell pkg-config --cflags sdl2 SDL2_ttf)
	SDL_LDFLAGS = $(shell pkg-config --libs sdl2 SDL2_ttf)
endif

CXXFLAGS = -std=c++17 -Wall -Wextra -O2 $(SDL_CFLAGS)
LDFLAGS = $(SDL_LDFLAGS)

# Target executable path
TARGET = $(BUILD_DIR)/PadTester

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# Default target
all: setup_sdl2 $(TARGET)

# Directory creation rules (Order-only prerequisites)
$(BUILD_DIR) $(OBJ_DIR) $(SDL_INC_DIR)/SDL2:
	@mkdir -p $@

# Create a safe copy of SDL2 headers inside the build directory
setup_sdl2: | $(SDL_INC_DIR)/SDL2
ifeq ($(CROSS_COMPILE),aarch64-linux-gnu-)
	@echo "Isolating SDL2 headers for cross-compilation..."
	@cp -u /usr/include/SDL2/*.h $(SDL_INC_DIR)/SDL2/ 2>/dev/null || true
	@cp -u /usr/include/aarch64-linux-gnu/SDL2/*.h $(SDL_INC_DIR)/SDL2/ 2>/dev/null || true
endif

# Build the executable
$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Compile object files
# Note: setup_sdl2 is added as a prerequisite to prevent race conditions during parallel builds (-j)
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | setup_sdl2 $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean the entire build directory (including isolated SDL2 headers)
clean:
	rm -rf $(BUILD_DIR)

# ==========================================
# DEPLOY CONFIGURATION (UPLOAD TO CONSOLE)
# ==========================================
SSH_USER ?= ark
SSH_HOST ?= 192.168.1.100
DEST_DIR = /roms/ports

deploy: $(TARGET)
	@which sshpass > /dev/null || (echo "Error: sshpass tool is missing. Install it using: sudo apt install sshpass" && exit 1)
	@if [ ! -f PadTester.sh ]; then echo "Error: Missing file PadTester.sh"; exit 1; fi
	@read -p "Console IP Address [$(SSH_HOST)]: " host_input; \
	HOST=$${host_input:-$(SSH_HOST)}; \
	read -p "SSH User [$(SSH_USER)]: " user_input; \
	USER=$${user_input:-$(SSH_USER)}; \
	read -s -p "SSH Password (hidden): " pass_input; echo ""; \
	echo "Establishing connection and creating directories..."; \
	sshpass -p "$$pass_input" ssh -o StrictHostKeyChecking=no $$USER@$$HOST "mkdir -p $(DEST_DIR)/PadTester"; \
	echo "Transferring the executable file..."; \
	sshpass -p "$$pass_input" scp -o StrictHostKeyChecking=no $(TARGET) $$USER@$$HOST:$(DEST_DIR)/PadTester/; \
	echo "Transferring the launch script..."; \
	sshpass -p "$$pass_input" scp -o StrictHostKeyChecking=no PadTester.sh $$USER@$$HOST:$(DEST_DIR)/; \
	echo "Successfully deployed. In EmulationStation, use the option: Update Games Lists"

.PHONY: all clean setup_sdl2 deploy