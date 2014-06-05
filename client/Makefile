CC = gcc

CLIENT = client

INCLUDE_DIR = include
LIB_DIR = lib
SOURCE_DIR = src
BUILD_DIR = build

# Create hard links to transfer.h and libtransfer.a if they don't already exist
TRANSFER_DIR = ../transfer
TRANSFER_INCLUDE_DIR = $(TRANSFER_DIR)/include
$(shell ln $(TRANSFER_INCLUDE_DIR)/transfer.h $(INCLUDE_DIR)/transfer.h &> /dev/null)
$(shell ln $(TRANSFER_DIR)/libtransfer.a $(LIB_DIR)/libtransfer.a &> /dev/null)

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

all: $(CLIENT)

$(CLIENT): $(OBJ) $(LIBS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIB)
#$(addprefix -l ,$(LIB))

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(CLIENT)

###############################