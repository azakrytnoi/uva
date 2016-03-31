#
#

CC = g++

#CFLAGS = -m64 -Wall -O2 -std=c++14
CFLAGS = -m64 -Wall -Wextra -Weffc++ -std=c++14 -g3 -fPIC $(UCFLAGS)

#
INCLUDES = -I. -I../headers

LFLAGS = 

LIBS = 
#LIBS = -lu119913 -lu10062 -lu10066 -lu100 -lu10194 -lu10219 -lu10220 -lu10226 -lu10258 -lu10301 -lu10305 \

SRCS = $(wildcard *.cpp)

#
#
OBJS = $(SRCS:.cpp=.o)

MAIN = libu1199.so

#
#

.PHONY: depend clean

Debug:    $(MAIN)
	@echo  done

all:    $(MAIN)
	@echo  done

$(MAIN): $(OBJS) 
	$(CC) -shared $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	ln $(MAIN) ../lib/$(MAIN)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

cleanDebug:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

clean:
	$(RM) *.o *.out *.orig *.bak *~ ../lib/$(MAIN) $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

u1199.o: u1199.h
