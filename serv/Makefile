CC = gcc

MODULES_DIR = ../modules
# include $(shell find $(MODULES_DIR) -type f -name Makefile)

TARGET = serv

INC = $(shell find $(MODULES_DIR) -type f -name '*.h')
LIB = $(shell find $(MODULES_DIR) -type f -name '*.a')
SRC = serv.c
OBJ = serv.o

CFLAGS = -g -Wall -std=c99
INCFLAGS = -I $(MODULES_DIR)/options -I $(MODULES_DIR)/transfer

# Add modules to lib path
LIBFLAGS = -L $(MODULES_DIR)/options -loptions -L $(MODULES_DIR)/transfer -ltransfer


# Rules #######################

all: modules $(TARGET)

modules:
	for DIR in $(MODULES_DIR)/*/; do \
		pushd $$DIR && make; \
		popd; \
	done

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LIBFLAGS) -o $@ $^

$(OBJ): $(SRC) $(INC) $(LIB)
	$(CC) $(CFLAGS) $(INCFLAGS) -c -o $@ $<

clean:
	for DIR in $(MODULES_DIR)/*/; do \
		pushd $$DIR && make clean; \
		popd; \
	done
	@rm $(TARGET)
	@rm $(OBJ)