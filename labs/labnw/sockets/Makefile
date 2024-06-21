# Variables
CXX = g++
CXXFLAGS = -Wall -Wextra -pedantic -pthread -std=c++2a -g -O3
# PROGRAMS = client server
CLIENT = client
SERVER = server
COMMON = common
CLIENT_O = client.o
SERVER_O = server.o
COMMON_O = common.o

PROGRAMS = $(CLIENT) $(SERVER)

# Targets and Dependencies

all: $(PROGRAMS)
$(CLIENT): $(CLIENT_O) $(COMMON_O)
	$(CXX) $(CXXFLAGS) -o $@ $^
$(SERVER): $(SERVER_O) $(COMMON_O)
	$(CXX) $(CXXFLAGS) -o $@ $^
$(CLIENT_O): $(CLIENT).cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(SERVER_O): $(SERVER).cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<
$(COMMON_O): $(COMMON).cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	rm -f $(PROGRAMS) $(CLIENT_O) $(SERVER_O) $(COMMON_O)