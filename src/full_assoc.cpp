#include <iostream>
#include <fstream>
#include <vector>
#include <string>

struct full_Entry {
	unsigned int valid;
	unsigned int tag;
};



void full_assoc(std::vector<std::string> list, std::fstream &outFile)
{
	int total, hit;
	unsigned int addr, tag, tag_mask, tag_shift, offset_mask;
	std::vector<std::string>::iterator it;
	std::string tmp;
	
	for(it = list.begin(); it != list.end(); it++)
	{
		tmp = *it;
		addr = std::stoul(tmp.substr(2), nullptr, 16);
		tag = (addr & tag_mask) >> tag_shift;
	}
}
