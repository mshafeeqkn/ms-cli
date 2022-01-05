OBJS = build/main.o build/cmd_map.o build/ms_log.o
EXEC=bin/ms-cli
CFLAGS = -g -Wall -Werror
CC = gcc
INCLUDE =
LIBS =
BUILD_DIR = build
BIN_DIR = bin


$(EXEC): $(BUILD_DIR) $(BIN_DIR) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -rf build/* bin/*

$(BUILD_DIR):
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)

build/%.o : src/%.c
	$(CC) $(INCLUDE) -c $< -o $@
