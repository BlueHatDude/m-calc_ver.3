CC=gcc
WARNINGS=-Wall -Werror -Wextra -pedantic -fsanitize=address -Wno-unused-function
STANDARD=-std=c18


.PHONY: app tests clean


app: mcalc3.o src/main.c
	$(CC) -o mcalc3 mcalc3.o src/main.c $(STANDARD) $(WARNINGS)
	rm *.o

mcalc3.o: src/mcalc3.c
	$(CC) -c src/mcalc3.c $(WARNINGS) $(STANDARD)

tests: src/tests.c mcalc3.o
	$(CC) -o tests src/tests.c mcalc3.o $(WARNINGS) $(STANDARD)

clean:
	rm mcalc3.o mcalc3 tests
