all: src/project2.cpp src/set_assoc.cpp src/direct_map.cpp src/full_assoc.cpp
	g++ src/set_assoc.cpp src/direct_map.cpp src/full_assoc.cpp src/project2.cpp -g -o project2
	
run: clean all
	./project2 traces/trace1.txt output.txt
	
checkmem: all
	valgrind ./project2 traces/trace1.txt output.txt

clean:
	rm -rf *.o
	rm -rf project2
	rm -rf output.txt
