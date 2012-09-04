OBJS = tracker.o
CC = gcc
CFLAGS = -O3 -Wall 
LDFLAGS = -lhidapi-hidraw

.PHONY: all
all: trackertest

trackertest: trackertest.o $(OBJS)
	$(CC) -o $@ trackertest.o $(OBJS) $(CFLAGS) $(LDFLAGS)

.PHONY: clean
clean:
	rm -f $(OBJS) trackertest.o trackertest
