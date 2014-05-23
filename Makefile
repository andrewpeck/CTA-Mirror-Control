# SJF

LIBOBJECTS =  GPIOInterface.o Overo.o MirrorControlBoard.o SpiInterface.o Layout.o mcspiInterface.o
#	  OmniORBHelper.o VSAssert.o VSDataConverter.o VSOptions.o 
ALLOBJECTS = cbc.o demo.o $(LIBOBJECTS)

MYCXXFLAGS = -Wall -O3 -I. -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS #-DNOCOSEVENT -IomniORB4
MYLDFLAGS  = 
MYLIBS     = #-lomniDynamic4 -lomniORB4 

#MYCXXFLAGS += -g -fno-inline 
#MYLDFLAGS  += -g

all: demo cbc

#sctelescope: sctelescope.o $(LIBOBJECTS)
#	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)
#	$(STRIP) $@

cbc: cbc.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)
#	$(STRIP) $@

demo: demo.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(MYCXXFLAGS) -c $<

.PHONY: clean tar

tar:	clean
	tar zcvf ../sctelescope-code-1.1.0.tar.gz *.hpp *.cpp Makefile

clean:
	$(RM) sctelescope cbc demo *.o *~
