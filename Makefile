CC = gcc
CFLAGS = -pipe -Wall -O2 -D_FORTIFY_SOURCE=2 -D_GLIBCXX_ASSERTIONS -g

SOURCE_DIR=src
OUTPUT_DIR=bin

speller: dictionary
	$(CC) $(CFLAGS) speller.c $(OUTPUT_DIR)/dictionary.o -o speller.exe

dictionary:
	$(CC) $(CFLAGS) -c $(SOURCE_DIR)/dictionary.c -o $(OUTPUT_DIR)/dictionary.o
