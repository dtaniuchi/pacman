PROGRAM = main
OBJS = main.o func1.o func2.o
CC = gcc
CFLAGS = -Wall -O2
LOADLIBES = -lm

$(PROGRAM): $(OBJS)
	$(CC) $(OBJS) -o $(PROGRAM) $(CFLAGS)

main.o: main.c
	$(CC) -c main.c

func1.o: func1.c
	$(CC) -c func1.c

func2.o: func2.c
	$(CC) -c func2.c

clean:
	rm -f $(PROGRAM) $(OBJS) *~ *.bak
