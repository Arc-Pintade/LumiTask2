CC = g++
ROOT = `root-config --cflags --glibs --ldflags`
SRC=$(shell ls ./src/*.cpp)
OBJ=$(SRC:.cpp=.o)
.PHONY: clean
.PHONY: clean-all
all: task2 
clean:
	rm -f ./src/*.o
clean-all:
	rm -f ./src/*.o task2
%.o: %.cpp
	$(CC) -c $(ROOT) -o $@ $<
task2: $(OBJ) 
	$(CC) $^ $(ROOT) -o $@  
