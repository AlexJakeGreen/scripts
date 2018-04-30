all:
	mkdir -p ./bin
	${CC} main.c emulator.c utils.c disassembler.c cpm.c -o ./bin/emul -Wall

test: all
	./bin/emul ./tests/prelim.com | tee tests.log

#./bin/emul ./test.com 0x100

clean:
	rm -rf ./bin

# Links
# http://pastraiser.com/cpu/i8080/i8080_opcodes.html
