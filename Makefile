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
	chmod 755 /usr/lib/$(TARGET)
	cp cbc.hpp  /usr/include/cbc.hpp
	chmod 644 /usr/include/cbc.hpp

tar: 
	tar czvf libcbc.tar.gz ../libcbc/*.cpp ../libcbc/*.hpp ../libcbc/Makefile

update: 
	rm master.zip ; wget --no-check-certificate https://github.com/andrewpeck/CTA-Mirror-Control/archive/master.zip && unzip -o master.zip && mv CTA-Mirror-Control-master/* .; mv CTA-Mirror-Control-master/.gitignore .; rmdir CTA-Mirror-Control-master ; rm master.zip
