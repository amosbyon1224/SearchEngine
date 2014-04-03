# Compiler and compiler flags
CC=g++
# For final
# CFLAGS=-Werror=all -std=c++11 -O2 -I.
# For debug
CFLAGS=-Wall -std=c++11 -g -I./lib

all: crawler

crawler:
	$(CC) trevor_src/crawler.cpp -o $@ $(CFLAGS)

# Probably not how I should implement clean!
.PHONY: clean

clean:
	-@rm crawler
