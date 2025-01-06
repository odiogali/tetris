CC = g++
CFLAGS = -std=c++17 -Wall -Wextra
LDFLAGS = -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl

exec: main.cpp
	$(CC) $(CFLAGS) -o exec main.cpp $(LDFLAGS)

clean:
	rm -f exec
