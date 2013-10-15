CC=g++
LIBS=-lGLEW -lGLU -lGL -lglut

LIB=./lib
SRC=./src
BIN=./bin
RES=./res
SHADERS=./shaders
PROG=demo1

compile:
	g++ -o $(BIN)/$(PROG) $(SRC)/*.cpp $(LIBS) -I$(LIB)

demo: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/exampleShader.vert \
		$(SHADERS)/exampleShader.frag

clean:
	rm -f $(BIN)/*
