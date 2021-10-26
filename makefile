# sunwait
#
# 08/12/2014  IFC  0.6  No changes since 0.5
# 02/05/2015  IFC  0.7  No changes since 0.5, still
# 10/03/2020  TLJ  0.9  Move headers to fix build on osx
#


C=gcc
CFLAGS=-c -Wall -std=c99 -O2 -Wextra -pedantic
LDFLAGS= -lm
SOURCES=sunwait.c sunriset.c print.c
HEADERS=sunwait.h sunriset.h print.h
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sunwait

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(C) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(C) $(CFLAGS) $< -o $@

clean:
	rm -f *.o sunwait


