CC = g++
CCFLAGS = -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17 -g

all: array_main

olcPixelGameEngine.o: olcPixelGameEngine.cc olcPixelGameEngine.hh
		$(CC) -c olcPixelGameEngine.cc $(CCFLAGS)

array_view.o: array_view.cc array_view.hh
		$(CC) -c array_view.cc $(CCFLAGS)

array_main.o: array_main.cc array_main.hh
		$(CC) -c array_main.cc $(CCFLAGS)

array_main: array_view.o array_main.o olcPixelGameEngine.o 
		$(CC) -o array_main olcPixelGameEngine.o array_main.o array_view.o  $(CCFLAGS) 

.PHONY: commit
commit:
		git checkout main
		git add *.cc *.hh .gitignore README.md Makefile
		git commit
		git push origin main

.PHONY: clean
clean:
		rm -f main *.o
		rm array_main