CXX = gcc
CC  = gcc
LD  = gcc

OBJECTS = getSysInfo.o map_lib.o
TARGET  = getSysInfo

#DEBUG_FLAGS = -DDEBUG
DEBUG_FLAGS =
#BASE_CFLAGS = -O2 -Wall -D_REENTRANT $(DEBUG_FLAGS)
###### "Segmentation fault" occured when level 2 optimization is specified !!! change to O0 as instead
BASE_CFLAGS = -O0 -Wall -D_REENTRANT $(DEBUG_FLAGS)
CXXFLAGS    = $(BASE_CFLAGS) -I . 
LDFLAGS     = -lpthread

.SUFFIXES: .c
.c.o:
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(TARGET)

debug: CXXFLAGS := -g -O0 -Wall
debug: $(TARGET)

prof: CXXFLAGS := -g -pg -O0 -Wall
prof: LDFLAGS  := -pg $(LDFLAGS)
prof: $(TARGET)

#TT = $(shell echo "$(OBJECTS)" | sed -e 's/\s/ $(EDIR)\//g')
$(TARGET): makefile $(OBJECTS)
	$(LD) -o $(TARGET) $(OBJECTS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJECTS)

