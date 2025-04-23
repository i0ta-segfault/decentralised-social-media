CXX = g++
CXXFLAGS = -std=c++11 -Wall -Iinclude
LIBS = -lraylib -lGL -lm -lpthread -ldl -lX11 -lcurl -lssl -lcrypto

SRC = $(wildcard src/*.cpp)
EXEC = main

all: $(EXEC)

$(EXEC): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(EXEC) $(LIBS)

clean:
	rm -f $(EXEC)