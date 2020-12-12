CC=g++
SRCS=$(wildcard *.cpp */*.cpp)
OBJS=$(patsubst %.cpp, %.o, $(SRCS))
FLAG=-g -std=c++11
NAME=$(wildcard *.cpp)
TARGET=$(patsubst %.cpp, %, $(NAME))

$(TARGET):$(OBJS)
	$(CC) -o $@ $^ $(FLAG)

%.o:%.cpp
	$(CC) -o $@ -c $< $(FLAG)

clean:
	rm -rf $(TARGET) $(OBJS)