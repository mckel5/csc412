# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++2a -g -O3
OBJECTS = tee.o
PROGRAMS = tee

# a good question to ask is what are CXX and CXXFLAGS?

# Targets and Dependencies - implement each of these below
all:
	$(MAKE) tee.o
	$(MAKE) tee

tee: $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(PROGRAMS) 

tee.o: *.cpp
	$(CXX) $(CXXFLAGS) -o $(OBJECTS) -c $<

clean:
	-rm $(OBJECTS) $(PROGRAMS)
