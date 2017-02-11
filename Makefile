all:
	mkdir -p ./bin
	${CC} main.c emulator.c utils.c debug.c disassembler.c -o ./bin/emul -Wall
#	${CC} dis.c emulator.c utils.c -o ./bin/disasm -Wall

test: all
	./bin/emul ./data/80186/add.bin

clean:
	rm -rf ./bin

# Links
# http://pastraiser.com/cpu/i8080/i8080_opcodes.html
