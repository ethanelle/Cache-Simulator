#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "entry.h"

void direct_mapped(std::vector<std::string> list, std::fstream &outFile);

void direct_mapped(std::vector<std::string> list, std::fstream &outFile)
{
	int total, hit, i, index;
	std::string tmp;
	long addr;
	std::vector<std::string>::iterator it;
	
	i = 1024;
	while(i < 32768)
	{
		Entry cache[i];
		for(int h = 0; h < i; h++)
			cache[h] = Entry();
		
		total = hit = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			tmp = *it;
			addr = std::stoul(tmp.substr(2, 11), nullptr, 16);
			index = addr % i;
			if(tmp[0] == 76) /* load instruction */
			{
				
			}else /* store instruction */
			{
				
			}
			
			total++;
		}
		i *= 2;
	}
	
}

int main(int arc, char const* argv[])
{
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
	
	/* end tests */
	
	outFile.close();
	return 0;
}
