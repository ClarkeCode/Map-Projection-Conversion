CC=g++
CFLAGS=-Iinclude
SRC=src/auxi.cpp src/lodepng.cpp src/main.cpp src/projections.cpp

all:
	$(CC) $(SRC) $(CFLAGS) --std=c++17 -o main && ./main

clean:
	rm -f resource/output/* main