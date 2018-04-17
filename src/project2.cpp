#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

extern void set_assoc(std::vector<std::string> list, std::fstream &outFile);
extern void direct_mapped(std::vector<std::string> list, std::fstream &outFile);
extern void full_assoc(std::vector<std::string> list, std::fstream &outFile);

int main(int arc, char const* argv[])
{
	clock_t t1, t2;
	t1 = clock();
	
	std::vector<std::string> list;
	std::fstream inFile, outFile;
	inFile.open(argv[1], std::ios::in);
	outFile.open(argv[2], std::ios::out | std::ios::app);
	
	std::string line;
	while(std::getline(inFile, line))
	{
		list.push_back(line);
	}
	inFile.close();
	
	/* begin tests */
	
	direct_mapped(list, outFile);
	set_assoc(list, outFile);
	full_assoc(list, outFile);
	
	/* end tests */
	
	t2 = clock();
	std::cout << "\nTIME:\t" << ((float)(t2-t1) / 1000000) << " seconds" << std::endl;
	
	outFile.close();
	return 0;
}
