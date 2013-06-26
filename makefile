cc=gcc -Iinclude/ -o

SRC = src/*.c include/*.h
TEST = test/*.c

_CORE = main.c backprop.c netBuilder.c random.c
CORE = $(patsubst %,src/%,$(_CORE))

_FILE_BASED = filePersistence.c fileInput.c
FILE_BASED = $(patsubst %,src/%,$(_FILE_BASED))
FILE_BASED_ARGS = file_based -DFLAG_REQUIRED=1,NUM_ARGS=4

all:
	@echo "Please enter a build type (file_based)!"

file_based: $(SRC)
	$(cc) $(FILE_BASED_ARGS) $(CORE) $(FILE_BASED)

test: $(SRC) $(TEST)
	$(cc) backprop_test test/backprop_test.c src/backprop.c src/random.c
	$(cc) netBuilder_test src/netBuilder.c test/netBuilder_test.c
	$(cc) filePersistence_test src/filePersistence.c src/netBuilder.c src/random.c test/filePersistence_test.c
	$(cc) fileInput_test src/fileInput.c src/netBuilder.c test/fileInput_test.c
	./backprop_test
	./netBuilder_test
	./filePersistence_test
	./fileInput_test

clean:
	rm -f *.exe *.stackdump
