NAME = Y86

CC = gcc
CFLAGS = -g -Wall

y86:
	$(CC) $(CFLAGS) -o $(NAME) *.c
