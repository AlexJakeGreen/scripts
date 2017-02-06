all:
	mkdir -p ./bin
	${CC} main.c emulator.c utils.c -o ./bin/emul -Wall
#	${CC} dis.c emulator.c utils.c -o ./bin/disasm -Wall

test: all
	./bin/emul ./tests/cpudiag.bin 0x100

clean:
	rm -rf ./bin

# Links
# http://pastraiser.com/cpu/i8080/i8080_opcodes.html
