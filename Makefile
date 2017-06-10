all: main
main: main.o Camera.o Shader_Loader.o Render_Utils.o Texture.o picopng.o 
	g++ -Wall -g -o main main.o Render_Utils.o Camera.o Shader_Loader.o Texture.o picopng.o -lm -lGL -lGLU -lglut -lGLEW

main.o: main.cpp
	g++ -Wall -g -c main.cpp -o main.o

Camera.o: Camera.cpp
	g++ -Wall -g -c Camera.cpp -o Camera.o

picopng.o: picopng.cpp
	g++ -Wall -g -c picopng.cpp -o picopng.o 

Render_Utils.o: Render_Utils.cpp Render_Utils.h
	g++ -Wall -g -c Render_Utils.cpp -o Render_Utils.o 

Shader_Loader.o: Shader_Loader.cpp
	g++ -Wall -g -c Shader_Loader.cpp -o Shader_Loader.o 

Texture.o: Texture.cpp
	g++ -Wall -g -c Texture.cpp -o Texture.o

clean:
	rm -f *.o main