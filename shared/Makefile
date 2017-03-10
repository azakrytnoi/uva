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
evaluator.o: ../u11111/u11111.h ../u11221/u11221.h ../u213/u213.h
evaluator.o: ../u306/u306.h ../u444/u444.h ../u458/u458.h ../u492/u492.h
evaluator.o: ../u10896/u10896.h ../u11541/u11541.h ../u865/u865.h
evaluator.o: ../u468/u468.h ../u10878/u10878.h ../u424/u424.h ../u465/u465.h
evaluator.o: ../u619/u619.h ../u12243/u12243.h ../u184/u184.h
evaluator.o: ../u1210/u1210.h ../u11223/u11223.h ../u10921/u10921.h
evaluator.o: ../u10851/u10851.h ../u11385/u11385.h ../u740/u740.h
evaluator.o: ../u855/u855.h ../u162/u162.h ../u119/u119.h ../u913/u913.h
evaluator.o: ../u10131/u10131.h ../u10010/u10010.h ../u10235/u10235.h
evaluator.o: ../u147/u147.h ../u357/u357.h ../u371/u371.h ../u324/u324.h
evaluator.o: ../u11159/u11159.h ../u10424/u10424.h ../u10252/u10252.h
evaluator.o: ../u484/u484.h ../u10161/u10161.h ../u10260/u10260.h
evaluator.o: ../u350/u350.h ../u583/u583.h ../u623/u623.h ../u568/u568.h
evaluator.o: ../u696/u696.h ../u10033/u10033.h ../u706/u706.h
evaluator.o: ../u10142/u10142.h ../u10189/u10189.h ../u12709/u12709.h
evaluator.o: ../u10267/u10267.h ../u10196/u10196.h ../u10137/u10137.h
evaluator.o: ../u10038/u10038.h ../u10315/u10315.h ../u10050/u10050.h
evaluator.o: ../u344/u344.h ../u294/u294.h ../u10205/u10205.h
evaluator.o: ../u10044/u10044.h ../u406/u406.h ../u11547/u11547.h
evaluator.o: ../u10141/u10141.h ../u10006/u10006.h ../u848/u848.h
evaluator.o: ../u10150/u10150.h ../u10132/u10132.h ../u10041/u10041.h
evaluator.o: ../u120/u120.h ../u10152/u10152.h ../u10138/u10138.h
evaluator.o: ../u10026/u10026.h ../u10191/u10191.h ../u10037/u10037.h
evaluator.o: ../u10035/u10035.h ../u10018/u10018.h ../u701/u701.h
evaluator.o: ../u847/u847.h ../u10127/u10127.h ../u10105/u10105.h
evaluator.o: ../u10077/u10077.h ../u10202/u10202.h ../u10110/u10110.h
evaluator.o: ../u850/u850.h ../u10104/u10104.h ../u10168/u10168.h
evaluator.o: ../u10139/u10139.h ../u10089/u10089.h ../u10042/u10042.h
evaluator.o: ../u10090/u10090.h ../u10001/u10001.h ../u861/u861.h
evaluator.o: ../u10181/u10181.h ../u10128/u10128.h ../u10160/u10160.h
evaluator.o: ../u10032/u10032.h ../u704/u704.h ../u11805/u11805.h
evaluator.o: ../u488/u488.h ../u11332/u11332.h ../u10079/u10079.h
evaluator.o: ../u11455/u11455.h ../u11727/u11727.h ../u11172/u11172.h
evaluator.o: ../u10071/u10071.h ../u10270/u10270.h ../u10783/u10783.h
evaluator.o: ../u10970/u10970.h ../u11984/u11984.h ../u10004/u10004.h
evaluator.o: ../u10067/u10067.h ../u10099/u10099.h ../u10029/u10029.h
evaluator.o: ../u10036/u10036.h ../u705/u705.h ../u10051/u10051.h
