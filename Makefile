CXX=g++
CXXFLAGS=--std=c++17 -Wall
CXXLIBS=-lallegro -lallegro_primitives -lallegro_font -lallegro_ttf
#Installation prefix
PREFIX=/usr/local/bin

EXTENSION=
ifeq ($(OS),Windows_NT)
	#Windows-specific code

	#Use .exe extension
    EXTENSION=.exe
else
	#Non-windows code

	#Use signals
	CXXFLAGS+=-DUSE_SIGNALS=1
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
