CC = gcc
MAKE = make

SERV = serv

INCLUDE_DIR = include
LIB_DIR = lib
SOURCE_DIR = src
BUILD_DIR = build
TRANSFER_DIR = ../transfer

# Flags
CFLAGS = -g -Wall -std=c99 -I $(INCLUDE_DIR) -L $(LIB_DIR)

# Header files
INC = $(shell find $(INCLUDE_DIR) -type f -name '*.h')

# Library files
LIB = $(shell find $(LIB_DIR) -type f -name '*.a')

# Source files
SRC = $(shell find $(SOURCE_DIR) -type f -name '*.c')

# Object files
OBJ = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRC))


# Rules #######################

all: $(SERV)

$(SERV): transfer $(OBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LIB)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

transfer:
	@cd $(TRANSFER_DIR) && $(MAKE)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(SERV)
	@cd $(TRANSFER_DIR) && $(MAKE) clean

###############################