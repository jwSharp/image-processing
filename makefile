
# compiler and its flags
CC = gcc
CFLAGS = -Wall -g
TARGET = exe

# run the program
all: compile link run

# compile the individual files
compile: main.o stenography.o
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o
stenography.o: stenography.c
	$(CC) $(CFLAGS) -c stenography.c -o stenography.o

# link the files together
link: main.o stenography.o
	$(CC) $(CFLAGS) main.o stenography.o -o $(TARGET)

# execute file
run: $(TARGET)
	./$(TARGET)

clean:
	rm -f *.o $(TARGET)