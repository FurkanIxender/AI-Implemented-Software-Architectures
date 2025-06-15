CC = gcc
CFLAGS = -Wall -pthread
OBJS = main.o counter.o task.o scheduler.o

all: cyclic_exec

cyclic_exec: $(OBJS)
	$(CC) $(OBJS) -o cyclic_exec $(CFLAGS)

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o cyclic_exec