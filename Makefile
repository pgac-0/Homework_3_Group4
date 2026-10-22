

CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra

SOURCES = main.cpp address.cpp credit_card.cpp person.cpp product.cpp transaction.cpp store.cpp

all: store_app


store_app: $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o store_app

run: store_app
	./store_app

clean:
	rm -f store_app
