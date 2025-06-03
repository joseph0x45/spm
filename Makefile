CC = gcc
CFLAGS = -Wall -Wextra -Iinclude -std=c17 -Werror -pedantic
SRC_DIR = src
OBJ_DIR = build
BIN = build/bin/spm.out
LIBS = -lcurl

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(BIN)

$(BIN): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)/bin

clean:
	rm -rf $(OBJ_DIR) $(BIN)

run: $(BIN)
	@./$(BIN) $(ARGS)

.PHONY: all clean run
