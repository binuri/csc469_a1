CC = gcc
CC_FLAG = -Wall

all : tracker

tsc.o: tsc.c
	$(CC) $(CC_FLAG) -std=gnu99 -c tsc.c

tracker.o : tracker.c
	$(CC) $(CC_FLAG) -std=gnu99 -c tracker.c

tracker: tracker.o tsc.o
	$(CC) $(CC_FLAG) -std=gnu99 -o tracker tracker.o tsc.o

clean:
	rm *.o tracker .*.swp 
 
