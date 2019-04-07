CXX=g++
CXXFLAGS=--std=c++17 -Wall
CXXLIBS=-lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
#Installation prefix
PREFIX=/usr/local/bin

#Dont use extension on GNU/Linux, exe on windows
EXTENSION=
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
install: pimontecarlo$(EXTENSION)
	install -m 755 $^ $(PREFIX)
