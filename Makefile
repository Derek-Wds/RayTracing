all: main.cpp
	g++ -std=c++17 -g -o main main.cpp

clean: 
	$(RM) main