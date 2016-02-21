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
CFLAGS = -m64 -Wall -std=c++14 $(UCFLAGS)

# define any directories containing header files other than /usr/include
#
INCLUDES = -I.

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = 

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = 

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
MAIN = uva

#
# The following part of the makefile is generic; it can be used to 
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

.PHONY: depend clean

all:    $(MAIN)
	@echo  done

$(MAIN): $(OBJS) 
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file) 
# (see the gnu make manual section about automatic variables)
.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<  -o $@

clean:
	$(RM) *.o *.out *.orig *.bak *~ $(MAIN)

depend: $(SRCS)
	makedepend $(INCLUDES) $^

# DO NOT DELETE THIS LINE -- make depend needs it

u10931.o: u10931.h
u100.o: u100.h
u908.o: u908.h
u10219.o: u10219.h
u200.o: u200.h
u11462.o: u11462.h
u544.o: u544.h
u10301.o: u10301.h
u727.o: u727.h
u146.o: u146.h
u10062.o: u10062.h
u10258.o: u10258.h
u628.o: u628.h
u11340.o: u11340.h
u10194.o: u10194.h
u116.o: u116.h
u11308.o: u11308.h
u11150.o: u11150.h
u673.o: u673.h
u305.o: u305.h
u10305.o: u10305.h
u10066.o: u10066.h
u594.o: u594.h
u11461.o: u11461.h
u10013.o: u10013.h
u11136.o: u11136.h
u156.o: u156.h
u714.o: u714.h
u127.o: u127.h
evaluator.o: evaluator.h uvas.h u100.h u10219.h u10258.h u10305.h u11136.h
evaluator.o: u11308.h u11388.h u11495.h u146.h u482.h u558.h u673.h u714.h
evaluator.o: u908.h u10194.h u10226.h u10301.h u10810.h u11239.h u11340.h
evaluator.o: u11462.h u11854.h u11636.h u10220.h u127.h u200.h u544.h u594.h
evaluator.o: u679.h u727.h u957.h u628.h u156.h u305.h u10013.h u11461.h
evaluator.o: u11150.h u167.h u116.h u10066.h u10931.h u11185.h u900.h
evaluator.o: u10062.h
u957.o: u957.h
u11854.o: u11854.h
u11239.o: u11239.h
u11388.o: u11388.h
u558.o: u558.h
u482.o: u482.h
u679.o: u679.h
u11185.o: u11185.h
u11495.o: u11495.h sort.h
u10226.o: u10226.h
u900.o: u900.h
u167.o: u167.h
u10220.o: u10220.h
u10810.o: u10810.h sort.h
u11636.o: u11636.h
