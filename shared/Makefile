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
CFLAGS = -m64 -Wall -Wextra -Weffc++ -std=c++14 -g3 -pthread $(UCFLAGS)

# define any directories containing header files other than /usr/include
#
INCLUDES = -I. -I../headers

# define library paths in addition to /usr/lib
#   if I wanted to include libraries not in /usr/lib I'd specify
#   their path using -Lpath, something like:
LFLAGS = -L../lib

# define any libraries to link into executable:
#   if I want to link in libraries (libx.so or libx.a) I use the -llibname 
#   option, something like (this will link in libmylib.so and libm.so:
LIBS = -ldl
#LIBS = -lu10013 -lu10062 -lu10066 -lu100 -lu10194 -lu10219 -lu10220 -lu10226 -lu10258 -lu10301 -lu10305 \
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
MAIN = starter

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
	$(CC) $(CFLAGS) $(INCLUDES) -o $(MAIN) $(OBJS) $(LFLAGS) $(LIBS)

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

evaluator.o: /usr/include/dlfcn.h /usr/include/features.h
evaluator.o: /usr/include/stdc-predef.h evaluator.h uvas.h ../u100/u100.h
evaluator.o: ../u10219/u10219.h ../u10258/u10258.h ../u10305/u10305.h
evaluator.o: ../u11136/u11136.h ../u11308/u11308.h ../u11388/u11388.h
evaluator.o: ../u11495/u11495.h ../u146/u146.h ../u482/u482.h ../u558/u558.h
evaluator.o: ../u673/u673.h ../u714/u714.h ../u908/u908.h ../u10194/u10194.h
evaluator.o: ../u10226/u10226.h ../u10301/u10301.h ../u10810/u10810.h
evaluator.o: ../u11239/u11239.h ../u11340/u11340.h ../u11462/u11462.h
evaluator.o: ../u11854/u11854.h ../u11636/u11636.h ../u11764/u11764.h
evaluator.o: ../u10220/u10220.h ../u127/u127.h ../u200/u200.h ../u544/u544.h
evaluator.o: ../u594/u594.h ../u679/u679.h ../u727/u727.h ../u957/u957.h
evaluator.o: ../u628/u628.h ../u156/u156.h ../u305/u305.h ../u10013/u10013.h
evaluator.o: ../u11461/u11461.h ../u11150/u11150.h ../u167/u167.h
evaluator.o: ../u116/u116.h ../u10066/u10066.h ../u10931/u10931.h
evaluator.o: ../u11185/u11185.h ../u900/u900.h ../u10062/u10062.h
evaluator.o: ../u478/u478.h ../u454/u454.h ../u412/u412.h ../u400/u400.h
evaluator.o: ../u401/u401.h ../u408/u408.h ../u417/u417.h ../u403/u403.h
evaluator.o: ../u713/u713.h ../u10369/u10369.h ../u10757/u10757.h
evaluator.o: ../u624/u624.h ../u10746/u10746.h ../u1199/u1199.h
evaluator.o: ../u11221/u11221.h ../u213/u213.h ../u306/u306.h ../u444/u444.h
evaluator.o: ../u458/u458.h ../u492/u492.h ../u10896/u10896.h
evaluator.o: ../u11541/u11541.h ../u865/u865.h ../u468/u468.h
evaluator.o: ../u10878/u10878.h ../u424/u424.h ../u465/u465.h ../u619/u619.h
