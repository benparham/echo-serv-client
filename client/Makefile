CC = gcc

MODULES_DIR = ../modules

TARGET = client

INC = $(shell find $(MODULES_DIR) -type f -name '*.h')
LIB = $(shell find $(MODULES_DIR) -type f -name '*.a')
SRC = client.c
OBJ = client.o

CFLAGS = -g -Wall -std=c99
INCFLAGS = -I $(MODULES_DIR)/options -I $(MODULES_DIR)/transfer
LIBFLAGS = -L $(MODULES_DIR)/options -loptions -L $(MODULES_DIR)/transfer -ltransfer


# Rules #######################

all: modules $(TARGET)

# Makes all modules
modules:
	@for DIR in $(MODULES_DIR)/*/; do \
		pushd $$DIR &> /dev/null && make; \
		popd &> /dev/null; \
	done

# Makes the client executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) -o $@ $^

# Makes the client object files
$(OBJ): $(SRC) $(INC) $(LIB)
	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ $<

clean:
	@for DIR in $(MODULES_DIR)/*/; do \
		pushd $$DIR &> /dev/null && make clean; \
		popd &> /dev/null; \
	done
	@rm $(TARGET)
	@rm $(OBJ)