CC = gcc
CFLAGS = -c

all:
	$(CC) mapper.c -o mapper
	$(CC) reducer.c -o reducer -lm
	$(CC) combiner.c -o combiner

mapper: mapper.c
	$(CC) $(CFLAGS) mapper.c
	$(CC) mapper.c -o mapper

reducer: reducer.c
	$(CC) $(CFLAGS) reducer.c -lm
	$(CC) reducer.c -o reducer -lm

combiner: combiner.c
	$(CC) $(CFLAGS) combiner.c
	$(CC) combiner.c -o combiner
