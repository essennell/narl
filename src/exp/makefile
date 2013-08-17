INCLUDE=-Ixri -I../../externals/catch/single_include
SRC=test.cpp
OBJS=test.o

%.o: %.cpp 
	g++ -std=c++11 -c -o $@ $< -Wall $(INCLUDE)

test.exe: $(OBJS)
	g++ -std=c++11 -o $@ $^

tests: clean test.exe
	.\test.exe

clean:
	-rm *.o
	-rm *.exe
