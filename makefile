FLAGS = -g -Wall -std=c++17

SOURCES = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJECTS = $(wildcard *.o)

chess: $(SOURCES) $(HEADERS)
			g++ $(FLAGS) $(SOURCES) -o chess

.PHONY: clean

clean:
	rm -f chess $(OBJECTS)