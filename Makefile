CC = g++
CCFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

all: main

olcPixelGameEngine.o: olcPixelGameEngine.cc olcPixelGameEngine.hh
		$(CC) -c olcPixelGameEngine.cc $(CCFLAGS)

array_view.o: array_view.cc array_view.hh
		$(CC) -c array_view.cc $(CCFLAGS)

main.o: main.cc main.hh
		$(CC) -c main.cc $(CCFLAGS)

main: array_view.o main.o olcPixelGameEngine.o 
		$(CC) -o main olcPixelGameEngine.o main.o array_view.o  $(CCFLAGS) 

.PHONY: commit
commit:
		git checkout main
		git add *.cc *.hh .gitignore README.md Makefile
		git commit
		git push origin main

.PHONY: clean
clean:
		rm -f main *.o