#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

extern void set_assoc(std::vector<std::string> list, std::fstream &outFile);

struct Direct_Entry {
	unsigned int valid;
	unsigned int tag;
};

void roundSelect(unsigned int &cacheSize, unsigned int &index_mask, unsigned int &tag_mask)
{
	if(cacheSize == 256)
	{
		cacheSize = 1024;
		index_mask = 0x00000ffc;
		tag_mask   = 0xfffff000;
	}else if(cacheSize == 1024)
	{
		cacheSize = 4096;
		index_mask = 0x00003ffc;
		tag_mask   = 0xffffc000;
	}else if(cacheSize == 4096)
	{
		cacheSize = 8192;
		index_mask = 0x00007ffc;
		tag_mask   = 0xffff8000;
	}else
	{
		cacheSize = 9999;
	}
}

void direct_mapped(std::vector<std::string> list, std::fstream &outFile)
{
	//Counters
	int total, hit;
	
	unsigned int cacheSize, index, tag, addr;

	// offset = 2
	unsigned int tag_mask   = 0xfffffc00;
	unsigned int index_mask = 0x000003fc;
	
	std::string tmp;
	std::vector<std::string>::iterator it;
	Direct_Entry *e;

	std::cout << "Actual:\t\t";
	
	cacheSize = 256;
	while(cacheSize <= 8192)
	{
		Direct_Entry cache[cacheSize];
	
		for(int j = 0; j < cacheSize; j++)
		{
			e = &(cache[j]);
			e->valid = 0;
			e->tag = 0;
		}
	
		total = hit = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			tmp = *it;
			addr = std::stoul(tmp.substr(2), nullptr, 16);
			index = addr & index_mask;
			index = index % cacheSize;
			tag = addr & tag_mask;
		
			e = &(cache[index]);
			if(tmp[0] == 'L')
			{
				if(e->valid == 1)
				{
					if(e->tag == tag)
					{
						hit++;
					}else
					{
						e->tag = tag;
					}
				}else{
					e->valid = 1;
					e->tag = tag;
				}
			}else if(tmp[0] == 'S')
			{
				hit++;
			}
			total++;
		}
		std::cout << hit << "," << total << "; ";
		outFile << hit << "," << total << "; ";
		
		roundSelect(cacheSize, index_mask, tag_mask);
	}
	
	std::cout << std::endl;
	outFile << std::endl;
	std::cout << "Desired:\t837589,1122102; 932528,1122102; 972661,1122102; 1005547,1122102;" << std::endl;
}

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
	
	/* end tests */
	t2 = clock();
	std::cout << "\nTIME:\t" << ((float)(t2-t1) / 1000000) << " seconds" << std::endl;
	
	outFile.close();
	return 0;
}
