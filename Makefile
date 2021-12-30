CC = gcc
FLAGS = -o
OUTPUT_NAME = boop

all:
	${CC} main.c ${FLAGS} ${OUTPUT_NAME}.exe
exec:
	${OUTPUT_NAME} --help
clean:
	del ${OUTPUT_NAME}.exe