OBJS = main.o cmd_map.o
CFLAGS = -g -Wall -Werror
CC = gcc
INCLUDE =
LIBS =

ms-cli: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LIBS)

clean:
	rm -rf *.o ms-cli

%.o : %.c
	$(CC) $(INCLUDE) -c $< -o $@
