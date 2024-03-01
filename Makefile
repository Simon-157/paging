CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -mcmodel=large 
INCLUDES = -Isrc/include

SRC_DIR = src/lib
OBJ_DIR = obj

SRC_FILES = $(wildcard $(SRC_DIR)/*.c) src/main.c
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

EXECUTABLE = paging 

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $(INCLUDES) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -g $(CFLAGS) $(INCLUDES) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(EXECUTABLE)

.PHONY: all clean