# sunwait
#
# 08/12/2014  IFC  0.6  No changes since 0.5
# 02/05/2015  IFC  0.7  No changes since 0.5, still
# 10/03/2020  TLJ  0.9  Move headers to fix build on osx
#


C=gcc
CFLAGS=-c -Wall 
LDFLAGS= -lm -lstdc++
SOURCES=sunwait.cpp sunriset.cpp print.cpp
HEADERS=sunwait.h sunriset.h print.h
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=sunwait

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(C) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(C) $(CFLAGS) $< -o $@

clean:
	rm -f *.o sunwait


