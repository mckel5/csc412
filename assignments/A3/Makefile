# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -std=c++2a -g -O3
OBJECTS = head.o pwd.o tail.o tee.o wc.o 412shell.o
PROGRAMS = head pwd tail tee wc 412shell

# Targets and Dependencies
all: $(OBJECTS) $(PROGRAMS)

head.o: head.cpp
	$(CXX) $(CXXFLAGS) -c $<

pwd.o: pwd.cpp
	$(CXX) $(CXXFLAGS) -c $<

tail.o: tail.cpp
	$(CXX) $(CXXFLAGS) -c $<

tee.o: tee.cpp
	$(CXX) $(CXXFLAGS) -c $<

wc.o: wc.cpp
	$(CXX) $(CXXFLAGS) -c $<

412shell.o: 412shell.cpp
	$(CXX) $(CXXFLAGS) -c $<

head: head.o
	$(CXX) $(CXXFLAGS) -o head $<

pwd: pwd.o
	$(CXX) $(CXXFLAGS) -o pwd $<

tail: tail.o
	$(CXX) $(CXXFLAGS) -o tail $<

tee: tee.o
	$(CXX) $(CXXFLAGS) -o tee $<

wc: wc.o
	$(CXX) $(CXXFLAGS) -o wc $<

412shell: 412shell.o
	$(CXX) $(CXXFLAGS) -o 412shell $<

clean:
	-rm -f $(OBJECTS) $(PROGRAMS)