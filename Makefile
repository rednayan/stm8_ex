#==============================================================================
# Makefile for building an STM8 project using SDCC and the Standard Peripheral Library (SPL)
# Target: STM8 Microcontrollers
#==============================================================================

#--- Directory Configuration ---
SRC_DIR = src
SPL_SRC_DIR = spl/src
INC_DIR = spl/inc
BUILD_DIR = build
TARGET_NAME = gpio_toggle
TARGET_HEX = $(BUILD_DIR)/$(TARGET_NAME).ihx # Output the final hex file to build/

#--- Compiler and Linker ---
CC = sdcc
# -I$(INC_DIR): Includes the header directory (spl/inc) for #include directives
# -I$(SRC_DIR): Includes the src directory
CFLAGS = -mstm8 --std-c99 -I. -I$(INC_DIR) -I$(SRC_DIR)

#--- Flashing Configuration ---
FLASH_TOOL = stm8flash
FLASH_PORT = stlinkv21
FLASH_DEVICE = stm8s208rb
# Now points to the target in the build directory
FLASH_FLAGS = -c $(FLASH_PORT) -p $(FLASH_DEVICE) -s flash -w $(TARGET_HEX)

#--- Source Files ---
# 1. Project-specific files (main logic and interrupt handlers) - FULL PATH
CORE_SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/stm8s_it.c

# 2. SPL driver files needed - FULL PATH
SPL_SRCS = $(SPL_SRC_DIR)/stm8s_gpio.c $(SPL_SRC_DIR)/stm8s_clk.c $(SPL_SRC_DIR)/stm8s_rst.c

# Combine all source files
ALL_SRCS = $(CORE_SRCS) $(SPL_SRCS)

# Generate a list of object files (.rel) in the BUILD_DIR from the source files (.c)
# Example: src/main.c -> build/main.rel
ALL_OBJS = $(patsubst %.c, $(BUILD_DIR)/%.rel, $(notdir $(ALL_SRCS)))

#--- Build Rules ---

# Default target: build the executable
.PHONY: all
all: $(TARGET_HEX)

# Rule 1: Link all object files into the final Intel Hex file (.ihx)
$(TARGET_HEX): $(ALL_OBJS)
	@mkdir -p $(BUILD_DIR) # Ensure build directory exists
	@echo "LNK Linking to $@"
	$(CC) $(CFLAGS) --out-fmt-ihx $^ -o $@

# Rule 2: Compile CORE source files (.c in src/) to objects (.rel in build/)
$(BUILD_DIR)/%.rel: $(SRC_DIR)/%.c
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Rule 3: Compile SPL source files (.c in spl/src/) to objects (.rel in build/)
$(BUILD_DIR)/%.rel: $(SPL_SRC_DIR)/%.c
	@echo "CC $<"
	$(CC) $(CFLAGS) -c $< -o $@

#--- Flash Rule ---
# Flashes the final firmware to the device
.PHONY: flash
flash: $(TARGET_HEX)
	@echo "Flashing $(TARGET_HEX) to $(FLASH_DEVICE)..."
	$(FLASH_TOOL) $(FLASH_FLAGS)

#--- Clean Rule ---
# Removes all generated files in the build directory
.PHONY: clean
clean:
	@echo "Cleaning up build files..."
	rm -f $(BUILD_DIR)/*.rel $(BUILD_DIR)/*.ihx $(BUILD_DIR)/*.asm $(BUILD_DIR)/*.lst $(BUILD_DIR)/*.sym $(BUILD_DIR)/*.map $(BUILD_DIR)/*.rst

#--- Erase Rule ---
# Wipes the entire program flash memory
.PHONY: erase
erase:
	@echo "Erasing entire flash memory on $(FLASH_DEVICE)..."
	$(FLASH_TOOL) -c $(FLASH_PORT) -p $(FLASH_DEVICE) -s flash -w erase
