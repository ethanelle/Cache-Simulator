#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>
#include "cache.h"

int main(int argc, char const* argv[])
{
	clock_t t1, t2;
	t1 = clock();

	std::vector<std::string> list;
	std::fstream inFile, outFile;
	inFile.open(argv[1], std::ios::in);

	std::string line;
	while(std::getline(inFile, line))
	{
		list.push_back(line);
	}
	inFile.close();

	Cache cache1(list, argv[2]);
	cache1.run_sim();

	t2 = clock();
	std::cout << "TIME:\t" << ((float)(t2-t1) / 1000000) << " seconds" << std::endl;
	return 0;
}