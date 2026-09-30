CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -O2 -Isrc
SRCS = src/quickSort.c src/mergeSort.c src/combSort.c

run: src/main.out
	./src/main.out

src/main.out: src/main.c $(SRCS) src/sort.h
	$(CC) $(CFLAGS) -o $@ src/main.c $(SRCS)

test: tests/test.out
	./tests/test.out

tests/test.out: tests/test_sort.c $(SRCS) src/sort.h
	$(CC) $(CFLAGS) -o $@ tests/test_sort.c $(SRCS)

csv: src/main.out
	./src/main.out --csv > report/results.csv

charts: csv
	python3 tools/plot.py

clean:
	rm -f src/main.out tests/test.out
