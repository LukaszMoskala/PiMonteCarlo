CXX=g++
CXXFLAGS=--std=c++17 -Wall
CXXLIBS=-lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
#Puste rozszerzenie dla GNU/Linux
EXTENSION=
#Dla windowsa .exe
ifeq ($(OS),Windows_NT)
    EXTENSION=.exe
endif

all: pimontecarlo$(EXTENSION)

pimontecarlo.o: main.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $^
pimontecarlo: pimontecarlo.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)
clean:
	rm -f pimontecarlo.o
clean-bin:
	rm -f pimontecarlo$(EXTENSION)
