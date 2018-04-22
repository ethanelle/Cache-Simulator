all: src/cache.cpp src/cache.h src/project2.cpp
	g++ src/cache.cpp src/project2.cpp -g -std=c++14 -o project2

run: clean all
	./project2 traces/trace1.txt output.txt
	
checkmem: all
	valgrind ./project2 traces/trace1.txt output.txt

clean:
	rm -rf *.o
	rm -rf project2
	rm -rf output.txt
