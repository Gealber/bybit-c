TARGET = example
SRC = src
LIBS = -lcurl -lssl -lcrypto
CC = gcc
CFLAGS = -g -Wall -O3

.PHONY: clean all default

default: $(TARGET)
all: default

OBJECTS = $(patsubst %.c, %.o, $(wildcard src/*.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $(subst src/, bin/, $@)

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	mkdir -p bin
	$(CC) $(CFLAGS) -c $(TARGET).c -o bin/$(TARGET).o
	$(CC) $(subst src/, bin/, $(OBJECTS)) bin/$(TARGET).o -Wall $(LIBS) -o bin/$@

build-run: clean default run clean-objs

valgrind-run:
	valgrind --leak-check=yes --track-origins=yes ./bin/example

run:
	./bin/example

clean-objs: ## remove objects built
	-rm -f bin/*.o

clean: ## clean all
	-rm -f bin/*.o
	-rm -f bin/$(TARGET)
