P=MySAG
OBJECTS= main.o
CFLAGS = -g -Wall -O3
LDLIBS=
CC=gcc
all: $(P)

$(P): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(P) $(OBJECTS)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o $(P)