CC := gcc
CFLAGS := -g -Wall -std=gnu23
LIBS := `pkg-config sdl3 --libs`

.PHONY: build
build:
	$(CC) $(CFLAGS) $(LIBS) -o build/sketcher src/main.c

run: build
	build/sketcher


clean:
	$(RM) build/*
