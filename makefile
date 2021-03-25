all: test

emulator: emulator.c
	gcc -Wall -Wextra $< -o $@

run: emulator
	./emulator

test: emulator
	./emulator

clean:
	rm -f ./emulator
