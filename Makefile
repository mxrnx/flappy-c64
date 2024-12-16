# Directories
SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := bin

# Compiler, assembler, and linker
CC := cc65
AS := ca65
LD := ld65

# Target platform
TARGET := c64

# Flags
CCFLAGS := -Ois -t $(TARGET) -T
ASFLAGS := --smart -t $(TARGET)
LDFLAGS := -t $(TARGET)

# Output files
PROGRAM := $(BIN_DIR)/flappybird.prg

# Source files
C_SOURCES := $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES := $(wildcard $(SRC_DIR)/*.s)

# Intermediate object files
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(C_SOURCES))
ASM_OBJECTS := $(patsubst $(SRC_DIR)/%.s, $(OBJ_DIR)/%.o, $(ASM_SOURCES))

# All object files
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

# Library
LIB := $(TARGET).lib

# Assembly files generated from C sources
GENERATED_ASM := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.s, $(C_SOURCES))

.PHONY: all
all: $(PROGRAM)

# Create necessary directories
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compile C sources to assembly
$(OBJ_DIR)/%.s: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CCFLAGS) -o $@ $<

# Assemble .s files from src to .o in obj
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Assemble .s files generated from .c files
$(OBJ_DIR)/%.o: $(OBJ_DIR)/%.s | $(OBJ_DIR)
	$(AS) $(ASFLAGS) -o $@ $<

# Link
$(PROGRAM): $(OBJECTS) | $(BIN_DIR)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LIB)
	chmod +x $@

.PHONY: clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Prevent implicit deletion of intermediate assembly files
.PRECIOUS: $(OBJ_DIR)/%.s

