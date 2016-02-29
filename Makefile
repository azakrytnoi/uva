
INCLUDES = -I. -I../headers


CXXFLAGS = -fpic -m64 -g3 -Weffc++ -Wextra -Wall -fmessage-length=0 -std=c++14

OBJS =		simple.o

LIBS =

TARGET =	simple

$(TARGET):	$(OBJS)
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
