CC=g++
LIBS=-lGLEW -lGLU -lGL -lglut

LIB=./lib
SRC=./src
BIN=./bin
RES=./res

all:
	g++ -o $(BIN)/demo1 $(SRC)/*.cpp $(LIBS) -I$(LIB)

run: all
	$(BIN)/demo1 $(RES)/teapot.obj

clean:
	rm -f $(BIN)/*
