CC = gcc
CFLAGS = -Wall -pthread
OBJS = main.o counter.o task.o scheduler.o

all: thread_scheduler

thread_scheduler: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o thread_scheduler

%.o: %.c
	$(CC) -c $< $(CFLAGS)

clean:
	rm -f *.o thread_scheduler