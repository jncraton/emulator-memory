all: test

emulator: emulator.c
	gcc -Wall -Wextra -Werror $< -o $@

run: emulator
	./emulator

test: emulator
	./emulator

clean:
	rm -f ./emulator
