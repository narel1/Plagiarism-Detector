CC = gcc


plagChecker:
	$(CC) -c plagChecker.c
	$(CC) -c hashmap.c
	$(CC) -o plagChecker plagChecker.o hashmap.o -lm

clean:
	rm plagChecker plagChecker.o hashmap.o 
