CC = gcc

SERV = serv

INCLUDE_DIR = include
SOURCE_DIR = src
BUILD_DIR = build

# Flags
CFLAGS = -g -Wall -std=c99 -I $(INCLUDE_DIR)

# Header files
# INC = $(shell find $(INCLUDE_DIR) -type f -name '*.h')
INC = 

# Source files
SERV_SRC = $(shell find $(SOURCE_DIR) -type f -name '*.c')

# Object files
SERV_OBJ = $(patsubst $(SOURCE_DIR)/%.c, $(BUILD_DIR)/%.o, $(SERV_SRC))


# Rules #######################

all: $(SERV)

$(SERV): $(SERV_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(INC)
	@mkdir -p $(dir $@)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(SERV)

###############################