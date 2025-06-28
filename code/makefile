CC=clang
STD=c11
EXES = mod2 mod3
CFLAGS_DEBUG = -g
CFLAGS_RELEASE = -g -O3
CFLAGS = -std=$(STD) $(CFLAGS_RELEASE)
HEADERS = util.h bench.h test.h
LINKS = util.o bench.o test.o

util.o: util.c util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) util.c -c -o util.o 

bench.o: bench.c bench.h util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) bench.c -c -o bench.o

test.o: test.c test.h util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) test.c -c -o test.o 

$(EXES): %: %.c $(LINKS) $(HEADERS)
	$(CC) $(CFLAGS) $< $(LINKS) -o $@ -lm

#mod2: mod2.c $(HEADERS)
#	$(CC) mod2.c -o mod2 $(CFLAGS)

#mod3: mod3.c $(HEADERS)
#	$(CC) mod3.o -o mod3 $(CFLAGS)

clean:
	-rm -f *.o
	-rm -f $(EXES)

# %.o: %.c
#	$(CC) -c -g -lm $< -o $@
