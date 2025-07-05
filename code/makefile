CC=clang
STD=c23
EXES = mod2 mod3 mod4 mod5
DEBUG_EXES := $(EXES:%=%.d)
TESTS = test_pool
CFLAGS_DEBUG = -g $(CFLAGS_BASE)
CFLAGS_RELEASE = -g -O3 $(CFLAGS_BASE)
CFLAGS_BASE = -std=$(STD) -Wall -Wpedantic
HEADERS = util.h bench.h test.h pool.h trees.h

COMMON = util.o bench.o test.o

util.o: util.c util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) util.c -c -o util.o 

bench.o: bench.c bench.h util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) bench.c -c -o bench.o

test.o: test.c test.h util.h
	$(CC) -std=$(STD) $(CFLAGS_RELEASE) test.c -c -o test.o 

%.d.o: %.c $(COMMON) $(HEADERS)
	$(CC) -c $(CFLAGS_DEBUG) $< -o $@

%.o: %.c $(COMMON) $(HEADERS)
	$(CC) -c $(CFLAGS_RELEASE) $< -o $@

$(DEBUG_EXES): %.d: %.c pool.d.o $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS_DEBUG) $< $(COMMON) pool.d.o -lm -o $@ 

$(EXES): %: %.c pool.o $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS_RELEASE) $< $(COMMON) pool.o -lm -o $@

test_pool: pool.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS_DEBUG) -DTESTING_POOL $< util.o test.o bench.o -lm

bench_pool: pool.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS_RELEASE) -DTESTING_POOL $< util.o test.o bench.o -lm

test_heap: heap.c $(COMMON) $(HEADERS)
	$(CC) $(CFLAGS_DEBUG) $< util.o test.o

#mod2: mod2.c $(HEADERS)
#	$(CC) mod2.c -o mod2 $(CFLAGS)

#mod3: mod3.c $(HEADERS)
#	$(CC) mod3.o -o mod3 $(CFLAGS)

clean:
	-rm -f *.o
	-rm -f $(EXES)
	-rm -f $(DEBUG_EXES)
	-rm -f $(TESTS)
	-rm -f a.out

# %.o: %.c
#	$(CC) -c -g -lm $< -o $@
