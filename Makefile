CXX = c++
CXXFLAGS = -std=c++2a -I/usr/local/mysql/include/
LDFLAGS = -L/usr/local/mysql/lib/ -lmysqlclient -lwiringPi

OBJS = main.o dht11.o dht22.o mysql_connector.o

all: amadoi

amadoi: $(OBJS)
	$(CXX) $(CXXFLAGS) -o amadoi $(OBJS) $(LDFLAGS)

main.o: main.cc dht11.h dht22.h mysql_connector.h
	$(CXX) $(CXXFLAGS) -c main.cc

dht11.o: dht11.cc dht11.h sensor.h
	$(CXX) $(CXXFLAGS) -c dht11.cc

dht22.o: dht22.cc dht22.h sensor.h
	$(CXX) $(CXXFLAGS) -c dht22.cc

mysql_connector.o: mysql_connector.cc mysql_connector.h
	$(CXX) $(CXXFLAGS) -c mysql_connector.cc

clean:
	rm -f *.o amadoi
