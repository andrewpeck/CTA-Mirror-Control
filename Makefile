CXX = g++
SOURCES = $(shell echo *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)

CXXFLAGS = -std=c++11 -fPIC -g -Wall -O3 -I.
LDFLAGS  = -shared

TARGET = libcbc.so

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean tar

clean:
	$(RM) *.o *.so

install: 
	cp $(TARGET) /usr/lib/$(TARGET)
	cp cbc.hpp  /usr/include/cbc.hpp
