CC=gcc
WARNINGS=-Wall -Werror -Wextra -pedantic
STANDARD=-std=c18


app: main.c mcalc3.o
	$(CC) -o mcalc3 mcalc3.o main.c $(STANDARD) $(WARNINGS)
	rm *.o


mcalc3.o:
	$(CC) -c mcalc3.c $(WARNINGS) $(STANDARD)

