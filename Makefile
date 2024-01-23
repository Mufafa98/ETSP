CC := g++
CFLAGS := -Wall -pthread -std=c++17
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

all: build run

build: 
	@$(CC) $(CFLAGS) $(LIBS) -I./include/ ./src/*.cpp -o ./bin/exe/ETSP

run:
	@./bin/exe/ETSP
clean:
	@rm -f server client
