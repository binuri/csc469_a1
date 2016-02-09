CC = gcc
CC_FLAG = -Wall

all : tracker context_tracker

tsc.o: tsc.c
	$(CC) $(CC_FLAG) -std=gnu99 -c tsc.c

tracker.o : tracker.c
	$(CC) $(CC_FLAG) -std=gnu99 -c tracker.c

tracker: tracker.o tsc.o
	$(CC) $(CC_FLAG) -std=gnu99 -o tracker tracker.o tsc.o

clean:
	rm *.o tracker context_tracker .*.swp *.out

context_tracker.o: context_tracker.c
	$(CC) $(CC_FLAG) -std=gnu99 -c context_tracker.c

context_tracker: context_tracker.o tsc.o
	$(CC) $(CC_FLAG) -std=gnu99 -o context_tracker context_tracker.o tsc.o

 
