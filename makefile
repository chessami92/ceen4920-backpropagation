cc=gcc -Iinclude/ -o

_DEPS = backprop.h netBuilder.h random.h
DEPS = $(patsubst %,include/%,$(_DEPS))

_SRC = backprop.c main.c netBuilder.c random.c
SRC = $(patsubst %,src/%,$(_SRC))

_TEST = backprop_test.c netBuilder_test.c
TEST = $(patsubst %,test/%,$(_TEST))

all: build

build: $(DEPS) $(SRC)
	$(cc) main src/main.c src/backprop.c src/netBuilder.c src/random.c

run: build
	./main.exe

test: $(DEPS) $(SRC) test/backprop_test.c
	$(cc) backprop_test test/backprop_test.c src/backprop.c src/random.c
	$(cc) netBuilder_test src/netBuilder.c test/netBuilder_test.c
	$(cc) filePersistence_test src/filePersistence.c src/netBuilder.c src/random.c test/filePersistence_test.c
	./backprop_test
	./netBuilder_test
	./filePersistence_test

clean:
	rm -f *.exe *.stackdump

.PHONY: all build run test clean
