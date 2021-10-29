# sunwait
#
# 08/12/2014  IFC  0.6  No changes since 0.5
# 02/05/2015  IFC  0.7  No changes since 0.5, still
# 10/03/2020  TLJ  0.9  Move headers to fix build on osx
#
CC=gcc
CFLAGS=-c -Wall -std=c99 -O2 -Wextra -pedantic
LDFLAGS= -lm
SOURCES=sunwait.c sunriset.c print.c
OBJECTS=$(SOURCES:.c=.o)

all: sunwait

sunwait: $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f *.o sunwait
