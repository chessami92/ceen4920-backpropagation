# Which compiler and options to use.
cc=gcc -o

all: build

build: main.c backprop.c random.c
	$(cc) main main.c backprop.c random.c

run: build
	./main.exe

test: backprop_test.c backprop.c random.c
	$(cc) backprop_test backprop_test.c backprop.c random.c
	$(cc) netBuilder_test netBuilder.c netBuilder_test.c
	./backprop_test
	./netBuilder_test

clean:
	rm -f *.exe *.stackdump
