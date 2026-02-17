# Configurations

MCU 	= atmega328p
F_CPU 	= 16000000

CC 		= avr-gcc
CPP 	= avr-g++
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
AVRSIZE = avr-size

PORT	= COM4
BAUD	= 115200
PROGRAMMER = arduino

TARGET 	= app
BUILD 	= build
SRC_DIR	= src
CLASS_DIR = classes
INC_DIR = include

# Compilation flags

CFLAGS	= -mmcu=$(MCU) -std=c99 -Wall -Wextra -Os -DF_CPU=$(F_CPU)
CFLAGS	+= -I$(INC_DIR)

CPPFLAGS = -mmcu=$(MCU) -Wall -Wextra -Os -DF_CPU=$(F_CPU)
CPPFLAGS += -I$(INC_DIR) -I$(CLASS_DIR)

LDFLAGS = -mmcu=$(MCU)

# Files

C_SRC = $(wildcard $(SRC_DIR)/*.c)
CPP_SRC = $(wildcard $(SRC_DIR)/*.cpp) $(wildcard $(CLASS_DIR)/*.cpp)

C_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o,$(C_SRC))

CPP_OBJ_1 = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD)/%.o, $(wildcard $(SRC_DIR)/*.cpp))
CPP_OBJ_2 = $(patsubst $(CLASS_DIR)/%.cpp, $(BUILD)/%.o, $(wildcard $(CLASS_DIR)/*.cpp))
CPP_OBJ = $(CPP_OBJ_1) $(CPP_OBJ_2)

OBJ = $(C_OBJ) $(CPP_OBJ)

ELF = $(BUILD)/$(TARGET).elf
HEX = $(BUILD)/$(TARGET).hex

# Build rules

all: $(HEX)

$(BUILD):
	mkdir -p $(BUILD)

# .c -> .o
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# .cpp -> .o
$(BUILD)/%.o: $(SRC_DIR)/%.cpp | $(BUILD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(BUILD)/%.o: $(CLASS_DIR)/%.cpp | $(BUILD)
	$(CPP) $(CPPFLAGS) -c $< -o $@

# Link
$(ELF): $(OBJ)
	$(CPP) $(LDFLAGS) $^ -o $@

# .out -> .hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -j .text -j .data $< $@

flash: $(HEX)
	$(AVRDUDE) -v -V -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

size: $(ELF)
	$(AVRSIZE) --mcu=$(MCU) --format=avr $(ELF)

clean:
	rm -rf $(BUILD)