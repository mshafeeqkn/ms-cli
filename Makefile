OBJS = build/main.o build/cmd_map.o
EXEC=bin/ms-cli
CFLAGS = -g -Wall -Werror
CC = gcc
INCLUDE =
LIBS =

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -rf build/*.o $(EXEC)

build/%.o : src/%.c
	$(CC) $(INCLUDE) -c $< -o $@
