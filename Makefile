out=sw_plug

CC=gcc
CFLAGS = -Wall -O2
INCLUDE=./include
OBJECT=src/swlog.o src/swcommon.o src/swplug.o

all: ${OBJECT}
	$(CC)  $^ -o $(out)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE)

clean:
	rm $(out) src/*.o -rf
