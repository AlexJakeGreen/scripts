all:
	mkdir -p ./bin
	${CC} main.c emulator.c utils.c cpm.c log.c -o ./bin/emul -Wall -O2
	${CC} test.c emulator.c utils.c cpm.c log.c -o ./bin/test -Wall -O2
	${CC} zx.c emulator.c utils.c cpm.c display.c -o ./bin/zx -Wall -O2 -lSDL2

test: all
	./bin/test

#./bin/emul ./test.com 0x100

clean:
	rm -rf ./bin

# Links
# http://pastraiser.com/cpu/i8080/i8080_opcodes.html
