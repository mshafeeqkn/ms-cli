OBJS = build/cmd_map.o\
    build/ms_log.o\
    build/ms_console.o

EXEC=bin/ms-cli
CFLAGS = -g -Wall -Werror
CC = gcc
INCLUDE =
LIBS =
BUILD_DIR = build
BIN_DIR = bin
TEST_EXEC=bin/test_cli
TEST_OBJ=build/test_cli.o
EXEC_OBJ=build/main.o

all: $(EXEC)
test: $(TEST_EXEC)

$(TEST_EXEC): $(BUILD_DIR) $(BIN_DIR) $(TEST_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(TEST_OBJ) $(OBJS) $(LIBS)

$(EXEC): $(BUILD_DIR) $(BIN_DIR) $(EXEC_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(EXEC_OBJ) $(OBJS) $(LIBS)

clean:
	rm -rf build/* bin/*

$(BUILD_DIR):
	[ -d $(BUILD_DIR) ] || mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	[ -d $(BIN_DIR) ] || mkdir -p $(BIN_DIR)

build/%.o : src/%.c
	$(CC) $(INCLUDE) -c $< -o $@
