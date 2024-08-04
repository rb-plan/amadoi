CXX = g++
CXXFLAGS = -O3 -Wall -I/usr/local/include
LDFLAGS = -L/usr/local/lib -lwiringPi -lpthread -lm

SRC = main.cc dht11.cc dht22.cc
OBJ = $(SRC:.cc=.o)
BIN = amadoi

all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean

