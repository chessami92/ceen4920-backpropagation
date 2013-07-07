cc=gcc -Iinclude/ -o

SRC = src/*.c include/*.h
TEST = test/*.c

_CORE = main.c backprop.c netBuilder.c random.c
CORE = $(patsubst %,src/%,$(_CORE))

_FILE_BASED = filePersistence.c fileInput.c
FILE_BASED = $(patsubst %,src/%,$(_FILE_BASED))
FILE_BASED_ARGS = file_based -DFLAG_REQUIRED -DNUM_ARGS=4 -DPRINT_INPUTS -DPRINT_OUTPUTS

_TRIG_BASED = filePersistence.c trigInput.c
TRIG_BASED = $(patsubst %,src/%,$(_TRIG_BASED))
TRIG_BASED_ARGS = trig_based -DFLAG_REQUIRED -DNUM_ARGS=3 -DPRINT_OUTPUTS -DPRINT_DESIRED_OUTPUTS

all:
	@echo "Please enter a build type (file_based, trig_based)!"

file_based: $(SRC)
	$(cc) $(FILE_BASED_ARGS) $(CORE) $(FILE_BASED)

trig_based: $(SRC)
	$(cc) $(TRIG_BASED_ARGS) $(CORE) $(TRIG_BASED)

test_all: $(SRC) $(TEST)
	$(cc) backprop_test test/backprop_test.c src/backprop.c src/random.c
	$(cc) netBuilder_test src/netBuilder.c test/netBuilder_test.c
	$(cc) filePersistence_test src/filePersistence.c src/netBuilder.c src/random.c test/filePersistence_test.c
	$(cc) fileInput_test src/fileInput.c src/netBuilder.c test/fileInput_test.c
	$(cc) trigInput_test src/trigInput.c src/netBuilder.c test/trigInput_test.c
	./backprop_test
	./netBuilder_test
	./filePersistence_test
	./fileInput_test
	./trigInput_test

clean:
	rm -f *.exe *.stackdump
