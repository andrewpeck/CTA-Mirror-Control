# SJF

LIBOBJECTS =  Registers.o Overo.o MirrorControlBoard.o SpiInterface.o
#	  OmniORBHelper.o VSAssert.o VSDataConverter.o VSOptions.o 
ALLOBJECTS = sctelescope.o cbc.o demo.o uboot_conf.o $(LIBOBJECTS)

MYCXXFLAGS = -Wall -O3 -I. -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS #-DNOCOSEVENT -IomniORB4
MYLDFLAGS  = 
MYLIBS     = #-lomniDynamic4 -lomniORB4 

#MYCXXFLAGS += -g -fno-inline 
#MYLDFLAGS  += -g

all: demo sctelescope uboot_conf cbc

sctelescope: sctelescope.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)
#	$(STRIP) $@

cbc: cbc.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)
#	$(STRIP) $@

demo: demo.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)

uboot_conf: uboot_conf.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)

SpiInterface: SpiInterface.o $(LIBOBJECTS)
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)

iotest: iotest.o Registers.o
	$(CXX) $(LDFLAGS) $(MYLDFLAGS) -o $@ $^ $(MYLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(MYCXXFLAGS) -c $<


.PHONY: clean tar

tar:	clean
	tar zcvf ../sctelescope-code-1.1.0.tar.gz *.hpp *.cpp Makefile

clean:
	$(RM) sctelescope cbc demo uboot_conf *.o *~
