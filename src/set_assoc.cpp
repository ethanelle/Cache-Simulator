#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>

struct Assoc_Entry {
	unsigned int valid;
	unsigned int tag;
	time_t timestamp;
};

void set_assoc(std::vector<std::string> list, std::fstream &outFile)
{
	std::cout << "\nActual:\t\t";
	int total, hit;
	unsigned int cacheSize, cacheAssoc, index, tag, addr;
	unsigned int offset_mask = 0x00000003;
	unsigned int index_mask  = 0x00001ffc;
	unsigned int tag_mask    = (0xffffffff - index_mask) - offset_mask;
	Assoc_Entry *e;
	
	std::string tmp;
	std::vector<std::string>::iterator it;
	
	cacheAssoc = 2;
	cacheSize = 2048;
	while(cacheAssoc <= 16)
	{	
/*		Entry cache[index][way]				*/
		Assoc_Entry cache[cacheSize][cacheAssoc];
		for(int i = 0; i < cacheSize; i++)
		{
			for(int j = 0; j < cacheAssoc; j++)
			{
				e = &(cache[i][j]);
				e->valid = 0;
				e->tag = 0;
				time(&(cache[i][j].timestamp));
			}
		}
		
		total = hit = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			tmp = *it;
			addr = std::stoul(tmp.substr(2), nullptr, 16);
			index = addr & index_mask;
			index %= cacheSize;
			tag = addr & tag_mask;

			int i;
			bool found = false;
			for(i = 0; i < cacheAssoc; i++)
			{
				e = &(cache[index][i]);
				if(e->valid == 1)
				{
					if(e->tag == tag)
					{
						hit++;
						time(&(cache[index][i].timestamp));
						found = true;
						break;
					}
				}else
				{
					e->valid = 1;
					e->tag = tag;
					time(&(cache[index][i].timestamp));
					found = true;
					break;
				}
			}
			if(!found)
			{
				int oldest_index = 0;
				for(int i = 1; i < cacheAssoc; i++)
				{
					if(cache[index][i].timestamp < cache[index][oldest_index].timestamp)
						oldest_index = i;
				}
				e = &(cache[index][oldest_index]);
				e->valid = 1;
				e->tag = tag;
				time(&(cache[index][oldest_index].timestamp));
			}
			
			total++;
		}
		
		cacheAssoc *= 2;
		cacheSize /= 2;
		
		if(cacheAssoc == 4)
			index_mask = 0xffc;
		else if(cacheAssoc == 8)
			index_mask = 0x7fc;
		else if(cacheAssoc == 16)
			index_mask = 0x3fc;
		
		tag_mask = (0xffffffff - index_mask) - offset_mask;
		
		std::cout << hit << "," << total << "; ";
		outFile << hit << "," << total << "; ";
	}
	std::cout << std::endl;
	outFile << std::endl;
	
	std::cout << "Desired:\t993999,1122102; 999852,1122102; 999315,1122102; 1000092,1122102;" << std::endl;
}
