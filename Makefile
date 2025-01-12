CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
GLADPATH = ~/builds/glad/src/glad.c
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

exec: main.cpp
	$(CC) $(CFLAGS) -o exec main.cpp Shader.cpp $(GLADPATH) $(LDFLAGS)

clean:
	rm -f exec
