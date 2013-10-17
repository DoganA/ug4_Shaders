CC=g++
LIBS=-lGLEW -lGLU -lGL -lglut
MKDIR=mkdir

LIB=./lib
SRC=./src
BIN=./bin
RES=./res
SHADERS=./shaders
PROG=main

bindir:
	$(MKDIR) -p $(BIN)

compile: bindir
	$(CC) -o $(BIN)/$(PROG) $(SRC)/*.cpp $(LIBS) -I$(LIB)

flat: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/simpleShader.vert \
		$(SHADERS)/simpleShader.frag \
		0

gouraud: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/simpleShader.vert \
		$(SHADERS)/simpleShader.frag \
		1

phong: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/phongShader.vert \
		$(SHADERS)/phongShader.frag \
		1

toon: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/toonShader.vert \
		$(SHADERS)/toonShader.frag \
		1

depth: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/depthShader.vert \
		$(SHADERS)/depthShader.frag \
		1

texture-flat: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/simpleShader.vert \
		$(SHADERS)/simpleShader.frag \
		0 \
		$(RES)/texture_map.bmp

texture-gouraud: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/simpleShader.vert \
		$(SHADERS)/simpleShader.frag \
		1 \
		$(RES)/texture_map.bmp

texture-phong: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/phongShader.vert \
		$(SHADERS)/phongShader.frag \
		1 \
		$(RES)/texture_map.bmp

bump: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/bumpmapShader.vert \
		$(SHADERS)/bumpmapShader.frag \
		1 \
		$(RES)/bump_map.bmp

environment: compile
	$(BIN)/$(PROG) $(RES)/teapot.obj \
		$(SHADERS)/environmentmapShader.vert \
		$(SHADERS)/environmentmapShader.frag \
		1 \
		$(RES)/sphere_map.bmp

clean:
	rm -f $(BIN)/*
