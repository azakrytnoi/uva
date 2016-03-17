#
# 'make depend' uses makedepend to automatically generate dependencies 
#               (dependencies are added to end of Makefile)
# 'make'        build executable file 'mycc'
# 'make clean'  removes all .o and executable files
#

# define the C compiler to use
CC = g++

# define any compile-time flags
#CFLAGS = -m64 -Wall -O2 -std=c++14
CFLAGS = -m64 -Wall -Wextra -Weffc++ -std=c++14 -g3 -fPIC $(UCFLAGS)

# define any directories containing header files other than /usr/include
#
INCLUDES = -I. -I../headers

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = 
#LIBS = -lu16713 -lu10062 -lu10066 -lu100 -lu10194 -lu10219 -lu10220 -lu10226 -lu10258 -lu10301 -lu10305 \
#	-lu10810 -lu10931 -lu11136 -lu11150 -lu11185 -lu11239 -lu11308 -lu11340 -lu11388 -lu11461 \
#	-lu11462 -lu11495 -lu11636 -lu116 -lu11854 -lu127 -lu146 -lu156 -lu167 -lu200 -lu305 -lu400 \
#	-lu408 -lu401 -lu412 -lu454 -lu478 -lu482 -lu544 -lu558 -lu594 -lu628 -lu673 -lu679 -lu714 \
#	-lu727 -lu900 -lu908 -lu957 -lu417 -lu403 -lu713

# define the C source files
SRCS = $(wildcard *.cpp)

# define the C object files 
#
# This uses Suffix Replacement within a macro:
#   $(name:string1=string2)
#         For each word in 'name' replace 'string1' with 'string2'
# Below we are replacing the suffix .c of all words in the macro SRCS
# with the .o suffix
#
OBJS = $(SRCS:.cpp=.o)

# define the executable file 
MAIN = libu167.so

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

Debug:    $(MAIN)
	@echo  done

all:    $(MAIN)
	@echo  done

$(MAIN): $(OBJS) 
	$(CC) -shared $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)
	cp $(MAIN) ../lib

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

cleanDebug:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

clean:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

u167.o: u167.h
