LIBS = -lGL -lSDL -lSDL_image -lGLU
INCLUDES = -I/usr/include/SDL
FLAGS = -g -Wall

all: src/darkcore.o src/generate.o src/main.c
	gcc $(LIBS) $(INCLUDES) $(FLAGS) obj/darkcore.o obj/generate.o src/main.c -o main

clean:
	find . -type f -name "*.so" -exec rm -f {} \;
	find . -type f -name "*.a" -exec rm -f {} \;
	find . -type f -name "*.o" -exec rm -f {} \;
	rm main

src/darkcore.o: src/darkcore.c src/darkcore.h
	gcc $(LIBS) $(INCLUDES) $(FLAGS) -c src/darkcore.c -o obj/darkcore.o
	
src/generate.o: src/generate.c src/generate.h
	gcc $(LIBS) $(INCLUDES) $(FLAGS) -c src/generate.c -o obj/generate.o

test: test_01.c
	gcc $(LIBS) $(INCLUDES) $(FLAGS) obj/darkcore.o src/test_01.c -o test_01
	./test_01

