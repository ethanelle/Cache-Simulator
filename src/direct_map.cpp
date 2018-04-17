#include <iostream>
#include <fstream>
#include <string>
#include <vector>

struct Direct_Entry {
	unsigned int valid;
	unsigned int tag;
};

void direct_mapped(std::vector<std::string> list, std::fstream &outFile)
{
	//Counters
	int total, hit;
	
	unsigned int cacheSize, index, tag, addr, offset_mask, index_mask, tag_mask, tag_shift;

	offset_mask = 0x00000003;
	index_mask  = 0x000003fc;
	tag_mask    = (0xffffffff - index_mask) - offset_mask;
	tag_shift   = 10;
	
	std::string tmp;
	std::vector<std::string>::iterator it;
	Direct_Entry *e;

	std::cout << "Actual:\t\t";
	
	cacheSize = 256;
	while(cacheSize <= 8192)
	{
		#if 0
		std::cout << "\n\nOFFSET: " << std::hex << offset_mask << "\nINDEX: " << index_mask << "\nTAG: " << tag_mask;
		std::cout << std::dec << "\nTAG SHIFT: " << tag_shift << std::endl << std::endl;
		#endif
		
		Direct_Entry cache[cacheSize];
	
		for(int j = 0; j < cacheSize; j++)
		{
			e = &(cache[j]);
			e->valid = 0;
			e->tag = 0;
		}
	
		tag_shift = 10;
		total = hit = 0;
		for(it = list.begin(); it != list.end(); it++)
		{
			tmp = *it;
			addr = std::stoul(tmp.substr(2), nullptr, 16);
			index = (addr & index_mask) >> 2;
			index = index % cacheSize;
			tag = (addr & tag_mask) >> tag_shift;
		
			e = &(cache[index]);
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
			#if 0
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
				if(e->valid == 1)
				{
					if(e->tag == tag)
					{
						hit++;
					}else
					{
						e->tag = tag;
					}
				}else
				{
					e->valid = 1;
					e->tag = tag;
				}
			}
			#endif
			total++;
		}
		if(cacheSize == 4096)
		{
			cacheSize *= 2;
		}else
		{
			cacheSize *= 4;
		}
		
		if(cacheSize == 1024)
		{
			index_mask = 0xffc;
			tag_shift = 12;
		}else if(cacheSize == 4096)
		{
			index_mask = 0x3ffc;
			tag_shift = 14;
		}else
		{
			index_mask = 0x7ffc;
			tag_shift = 15;
		}
		
		tag_mask = (0xffffffff - index_mask) - offset_mask;
		
		std::cout << hit << "," << total << "; ";
		outFile << hit << "," << total << "; ";
	}
	
	std::cout << std::endl;
	outFile << std::endl;
	std::cout << "Desired:\t837589,1122102; 932528,1122102; 972661,1122102; 1005547,1122102;" << std::endl;
}
