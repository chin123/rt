KOCIEMBA_SRC = coordcube.c cubiecube.c facecube.c search.c
RT_INCLUDE = include
CPPFLAGS = -std=c++14 -O3 -pthread
CFLAGS = -std=c99 -O3
BINDIR = bin
BIN = rt

rt: main.cpp kociemba/*.c
	mkdir -p $(BINDIR)
	gcc $(CFLAGS) kociemba/*.c -I$(RT_INCLUDE) -c
	g++ $(CPPFLAGS) -I$(RT_INCLUDE) *.o main.cpp -o $(BINDIR)/$(BIN)

