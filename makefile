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

flat: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/phongModel.vert \
		$(SHADERS)/phongModel.frag \
		0

gouraud: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/phongModel.vert \
		$(SHADERS)/phongModel.frag \
		1

toon: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/toonShader.vert \
		$(SHADERS)/toonShader.frag \
		1

clean:
	rm -f $(BIN)/*
