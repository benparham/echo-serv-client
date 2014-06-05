CC = gcc
AR = ar

TRANSFER = libtransfer.a

INCLUDE_DIR = include
SOURCE_DIR = src
BUILD_DIR = build

# Flags
CFLAGS = -g -Wall -std=c99 -I $(INCLUDE_DIR)
ARFLAGS = rvu

# Header files
INC = $(shell find $(INCLUDE_DIR) -type f -name '*.h')

# Source files
SRC = $(shell find $(SOURCE_DIR) -type f -name '*.c')

# Object files
OBJ = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))

# Rules #######################

all: $(TRANSFER)

$(TRANSFER): $(OBJ)
	$(AR) $(ARFLAGS) $@ $^
	ranlib $@

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(TRANSFER)

###############################