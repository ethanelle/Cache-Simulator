all: src/cache.cpp src/cache.h src/driver.cpp
	g++ src/cache.cpp src/driver.cpp -g -std=c++14 -o driver

run: clean all
	./driver traces/trace1.txt output.txt
	
checkmem: all
	valgrind ./driver traces/trace1.txt output.txt

clean:
	rm -rf *.o
	rm -rf driver
	rm -rf output.txt
