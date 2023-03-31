all: test

emulator: src/emulator.c src/emulator.h
	gcc -Wall -Wextra -Werror $< -o $@

assembler: src/assembler.c src/emulator.h
	gcc -Wall -Wextra -Werror $< -o $@

roms/%.bin: roms/%.asm assembler
	./assembler $< -o $@

run: emulator
	./emulator

test: emulator roms/memory-test.bin roms/array-sum.bin roms/projectile.bin roms/test-program.bin roms/fibonacci.bin 
	./emulator

clean:
	rm -f emulator assembler roms/*.bin
