CPPFLAGS=-g
LDFLAGS=-g

#CC=gcc
#CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFLAGS=-g
LDLIBS=

OBJS := $(patsubst %.cc,%.o,$(wildcard *.cc))

all: simulation

simulation: $(OBJS)
	$(CXX) $(LDFLAGS) -o simulation $(OBJS) $(LDLIBS) 

tool.o: tool.cc support.hh

support.o: support.hh support.cc

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) simulation