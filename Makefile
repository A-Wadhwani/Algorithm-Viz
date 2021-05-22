CC = g++
CCFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

all: main

main.o: main.cc olcPixelGameEngine.hh
		$(CC) -c main.cc $(CCFLAGS)

main: main.o
		$(CC) -o main main.o $(CCFLAGS) 

.PHONY: commit
commit:
		git checkout main
		git add *.cc *.hh .gitignore README.md Makefile
		git commit
		git push origin main

.PHONY: clean
clean:
		rm -f main *.o