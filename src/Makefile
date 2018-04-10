all: project2.cpp
	g++ project2.cpp -g -o project2
	
run: all
	./project2 ../traces/trace1.txt output.txt
	
checkmem: all
	valgrind ./project2 ../traces/trace1.txt output.txt

clean:
	rm -rf *.o
	rm -rf project2
	rm -rf output.txt
