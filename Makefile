CC=gcc
WARNINGS=-Wall -Werror -Wextra -pedantic 
STANDARD=-std=c18


.PHONY: app tests clean


app: mcalc3.o
	$(CC) -o mcalc3 mcalc3.o src/tests.c $(STANDARD) $(WARNINGS)
	rm *.o


mcalc3.o: src/mcalc3.c
	$(CC) -c src/mcalc3.c $(WARNINGS) $(STANDARD)


tests: src/tests.c mcalc3.o
	$(CC) -o mcalc3_tests src/tests.c mcalc3.o $(STANDARD)

clean:
	rm mcalc3.o mcalc3 mcalc3_tests
