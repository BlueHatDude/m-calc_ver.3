CC=gcc
WARNINGS=-Wall -Werror -Wextra -pedantic
STANDARD=-std=c18


.PHONY: app tests clean


app: main.c mcalc3.o
	$(CC) -o mcalc3 mcalc3.o main.c $(STANDARD) $(WARNINGS)
	rm *.o


mcalc3.o: mcalc3.c
	$(CC) -c mcalc3.c $(WARNINGS) $(STANDARD)


tests: tests.c mcalc3.o
	$(CC) -o mcalc3_tests tests.c mcalc3.o $(STANDARD)
	./mcalc3_tests

clean:
	rm mcalc3.o mcalc3 mcalc3_tests
