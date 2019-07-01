#
#

CC = g++-8

CFLAGS = -m64 -Wall -Wextra -Weffc++ -std=c++17 -g -fPIC $(UCFLAGS)

#
INCLUDES = -I. -I../../headers

LFLAGS = 

LIBS = 

SRCS = $(wildcard *.cpp)

#
#
OBJS = $(SRCS:.cpp=.o)

MAIN = libu10028.so

#
#

.PHONY: depend clean

Debug:    $(MAIN)
	@echo  done

all:    $(MAIN)
	@echo  done

$(MAIN): $(OBJS) 
	$(CC) -shared $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	ln -sf $(shell pwd)/$(MAIN) ../../lib/$(MAIN)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

cleanDebug:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

clean:
	$(RM) *.o *.out *.orig *.bak *~ ../../lib/$(MAIN) $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

u10028.o: u10028.h
