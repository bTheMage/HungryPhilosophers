CC := gcc
EXE := main

CFILES := main.c

DEBUG_FLAGS := -O0 -g3 -fno-inline
RELEASE_FLAGS := -O3

CFLAGS := $(DEBUG_FLAGS)

all:
	$(CC) $(CFLAGS) -o $(EXE) $(CFILES)

run:
	./$(EXE)
