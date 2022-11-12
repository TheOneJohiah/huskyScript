all: hscript.c
	gcc -o hscript hscript.c
debug: hscript.c
	gcc -Wall -Wextra -g -o hscript hscript.c
clean:
	rm hscript
submission:
	tar czvf prog4.tgz Makefile hscript.c README