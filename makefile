# author: Jacob Sharp

CC = gcc
CFLAGS = -Wall -g
TARGET = exe

# run the program
all: install-pipenv python compile link run

# when not found, install pipenv
install-pipenv:
	@echo "Checking if pipenv is installed..."
	@which pipenv || (echo "pipenv not found, installing..."; pip install pipenv)

# install Python dependencies and run script
python: install-python run-python

install-python:
	pipenv install

run-python:
	pipenv run python image.py

# compile the individual files
compile: main.o stenography.o

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

stenography.o: stenography.c
	$(CC) $(CFLAGS) -c stenography.c -o stenography.o

# link the files together
link: main.o stenography.o
	$(CC) $(CFLAGS) main.o stenography.o -o $(TARGET)

# execute Stenography driver
run: $(TARGET)
	./$(TARGET)

# clean targets
clean: clean-c clean-python

clean-c:
	rm -f *.o $(TARGET)

clean-python:
	-pipenv --rm
	-rm -f Pipfile.lock