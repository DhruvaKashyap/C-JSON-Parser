OBJS = C_JSON.o client.o
CFLAGS= -g -Wall
CC=gcc
a.out: $(OBJS) C_JSON.h
	$(CC) $(CFLAGS) $(OBJS)
$(OBJS): C_JSON.h
clean:
	rm a.out $(OBJS)
