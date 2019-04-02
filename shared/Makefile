#
#

CC = g++

CFLAGS = -m64 -Wall -Wextra -Weffc++ -std=c++14 -g3 -pthread $(UCFLAGS)

#
INCLUDES = -I. -I../headers

LFLAGS = -L../lib

LIBS = -ldl

SRCS = $(wildcard *.cpp)

#
#
OBJS = $(SRCS:.cpp=.o)

MAIN = starter

#
#

.PHONY: depend clean

Debug:    $(MAIN)
	@echo  done

all:    $(MAIN)
	@echo  done

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

cleanDebug:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

clean:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

evaluator.o: /usr/include/dlfcn.h /usr/include/features.h
evaluator.o: /usr/include/stdc-predef.h evaluator.h /usr/include/dirent.h
