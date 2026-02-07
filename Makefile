# Configurations

MCU 	= atmega328p
F_CPU 	= 16000000

CC 		= avr-gcc
OBJCOPY = avr-objcopy
AVRDUDE = avrdude
AVRSIZE = avr-size

PORT	= COM4
BAUD	= 115200
PROGRAMMER = arduino

TARGET 	= app
BUILD 	= build
SRC_DIR	= src
INC_DIR = include

# Compilation flags

CFLAGS	= -mmcu=$(MCU) -std=c99 -Wall -Wextra -Os -DF_CPU=$(F_CPU)
CFLAGS	+= -I$(INC_DIR)

LDFLAGS = -mmcu=$(MCU)

# Files

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD)/%.o,$(SRC))

ELF = $(BUILD)/$(TARGET).elf
HEX = $(BUILD)/$(TARGET).hex

# Build rules

all: $(HEX)

$(BUILD):
	mkdir -p $(BUILD)

# .c -> .o
$(BUILD)/%.o: $(SRC_DIR)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(ELF): $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# .out -> .hex
$(HEX): $(ELF)
	$(OBJCOPY) -O ihex -j .text -j .data $< $@

flash: $(HEX)
	$(AVRDUDE) -v -V -p $(MCU) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -D -U flash:w:$<:i

size: $(ELF)
	$(AVRSIZE) --mcu=$(MCU) --format=avr $(ELF)

clean:
	rm -rf $(BUILD)