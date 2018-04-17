#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <time.h>

struct full_Entry {
	unsigned int valid;
	unsigned int tag;
	time_t timestamp;
};

/*
	Each used entry is scanned over to see if it is valid.
	If the entry is valid, its tag is checked to see if it matches
	that of the entry we are looking for.
	If it matches, the hit and timestamp are recorded.
	If there is no match, it returns false telling the main LRU function that
	the entry is NOT in the cache and it is a miss.
*/
bool find_in_cache(full_Entry cache[], unsigned int free_index, unsigned int tag)
{
	for(int i = 0; i < free_index; i++)
	{
		full_Entry x = cache[i];
		if(x.valid == 1)
		{
			if(x.tag == tag)
			{
				time(&(cache[i].timestamp));
				return true;
			}
		}
	}
	return false;
}

/*
	Each entry goes into one big array that acts as the cache.
	The cache indexes based on availability instead of bits in the address.
	The tag now acts as the memory's sole indentifier.
	Each memory instrucion gets placed in order into the array until the cache
	is full.
	When the cache fills up, the least recently used entry is replaced
	with the current entry being handled.
*/
void full_assoc_LRU(std::vector<std::string> list, int &hit, int &total)
{
	unsigned int free_index, tag, addr, tag_mask;
	bool found;
	std::vector<std::string>::iterator it;
	std::string tmp;
	full_Entry *e;

	full_Entry cache[512];

	tag_mask = 0xfffffffc;
	free_index = 0;
	total = hit = 0;
	for(it = list.begin(); it != list.end(); it++)
	{
		tmp = *it;
		addr = std::stoul(tmp.substr(2), nullptr, 16);
		tag = (addr & tag_mask) >> 2;
		
		found = find_in_cache(cache, free_index, tag);
		if(found)
			hit++;
		else
		{
			if(free_index < 512)
			{
				e = &(cache[free_index]);
				e->tag = tag;
				e->valid = 1;
				time(&(cache[free_index].timestamp));
				free_index++;
			}else
			{
				int oldest_index = 0;
				for(int i = 1; i < 512; i++)
				{
					if(cache[i].timestamp < cache[oldest_index].timestamp)
						oldest_index = i;
				}
				e = &(cache[oldest_index]);
				e->tag = tag;
				e->valid = 1;
				time(&(cache[oldest_index].timestamp));
			}
		}

		total++;
	}
}

/*
	Basically the same thing as the LRU version, but you don't care about the
	timestamp that comes along with each entry
*/
void full_assoc_HC(std::vector<std::string> list, std::fstream &outFile);
{
	unsigned int free_index, tag, addr, tag_mask;
	bool found;
	std::vector<std::string>::iterator it;
	std::string tmp;
	full_Entry *e;

	full_Entry cache[512];

	tag_mask = 0xfffffffc;
	free_index = 0;
	total = hit = 0;
	for(it = list.begin(); it != list.end(); it++)
	{
		tmp = *it;
		addr = std::stoul(tmp.substr(2), nullptr, 16);
		tag = (addr & tag_mask) >> 2;
		
		found = find_in_cache(cache, free_index, tag);
		if(found)
			hit++;
		else
		{
			if(free_index < 512)
			{
				e = &(cache[free_index]);
				e->tag = tag;
				e->valid = 1;
				free_index++;
			}else
			{
				/* hot and cold stuff here */
			}
		}

		total++;
	}
}

/*
	Just hands off control to the respective full associativity function.
*/
void full_assoc(std::vector<std::string> list, std::fstream &outFile)
{
	int total, hit;
	total = hit = 0;
	full_assoc_LRU(list, hit, total);
	std::cout << "\nActual:\t\t";
	std::cout << hit << "," << total << ";" << std::endl;
	outFile << hit << "," << total << ";" << std::endl;
	std::cout << "Desired:\t1000500,1122102;" << std::endl;
#if 0	
	full_assoc_HC(list, hit, total);
	std::cout << hit << "," << total << ";";
	outFile << hit << "," << total << ";";
#endif
}
