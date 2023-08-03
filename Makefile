TARGET = example
SRC = src
LIBS = -lcurl
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
	$(CC) $(subst src/, bin/, $(OBJECTS)) -Wall $(LIBS) -o bin/$@

build-run: clean default run clean-objs

valgrind-run:
	valgrind --leak-check=yes ./bin/example

run:
	./bin/example

clean-objs: ## remove objects built
	-rm -f bin/*.o

clean: ## clean all
	-rm -f bin/*.o
	-rm -f bin/$(TARGET)
