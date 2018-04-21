#include "cache.h"
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <chrono>

struct Entry {
	int valid;
	int lowBound;
	int highBound;
};

struct LRU_Entry {
	int valid;
	int lowBound;
	int highBound;
	std::chrono::high_resolution_clock::time_point timestamp;
};

Cache::Cache(std::vector<std::string> list, std::string fileName)
{
	this->outFile.open(fileName, std::ios::out | std::ios::app);
	this->list = std::vector<std::string> (list);
}

Cache::~Cache()
{
	this->outFile.close();
}

void Cache::run_sim()
{
	int total = this->list.size();
	int hit;
#if 1
	/* DIRECT MAP */
	std::cout << "ACTUAL:\t\t";
	int cacheSize = 1024;
	while(cacheSize <= 32768)
	{
		hit = direct_map(cacheSize / 32);
		std::cout << hit << "," << total << "; ";
		this->outFile << hit << "," << total << "; ";

		if(cacheSize == 1024)
			cacheSize = 4096;
		else if(cacheSize == 4096)
			cacheSize = 16384;
		else if(cacheSize == 16384)
			cacheSize = 32768;
		else
		{
			std::cout << std::endl;
			this->outFile << std::endl;
			break;
		}
	}
	std::cout << "DESIRED:\t837589,1122102; 932528,1122102; 972661,1122102; 1005547,1122102;" << std::endl;
	
	std::cout << "\nACTUAL:\t\t";
	int cacheAssoc = 2;
	while(cacheAssoc <= 16)
	{
		hit = set_assoc(cacheAssoc);
		std::cout << hit << "," << total << "; ";
		this->outFile << hit << "," << total << "; ";

		cacheAssoc *= 2;
		if(cacheAssoc > 16)
		{
			std::cout << std::endl;
			this->outFile << std::endl;
			break;
		}
	}
	std::cout << "DESIRED:\t993999,1122102; 999852,1122102; 999315,1122102; 1000092,1122102;" << std::endl;
#endif
	std::cout << "\nACTUAL:\t\t";
	hit = full_assoc_LRU();
	std::cout << hit << "," << total << ";" << std::endl;
	this->outFile << hit << "," << total << ";" << std::endl;;
	std::cout << "DESIRED:\t1000500,1122102;" << std::endl;
}

int Cache::direct_map(int cacheSize)
{
	Entry cache[cacheSize] = {0};
	std::vector<std::string>::iterator it;
	int hit = 0;
	for(it = this->list.begin(); it != this->list.end(); it++)
	{
		std::string tmp = *it;
		int addr = std::stoul(tmp.substr(2), nullptr, 16);
		int index = std::floor(addr / 32);
		index %= cacheSize;
		if(cache[index].valid == 1)
		{
			if((cache[index].lowBound <= addr) && (cache[index].highBound >= addr))
				hit++;
			else
			{
				cache[index].lowBound = std::floor(addr / 32) * 32;
				cache[index].highBound = cache[index].lowBound + 31;
				cache[index].valid = 1;
			}
		}else
		{
			cache[index].lowBound = std::floor(addr / 32) * 32;
			cache[index].highBound = cache[index].lowBound + 31;
			cache[index].valid = 1;
			hit++;
		}
	}
	return hit;
}

int Cache::assoc_sizer(int cacheAssoc)
{
	if(cacheAssoc == 2)
		return 256;
	else if(cacheAssoc == 4)
		return 128;
	else if(cacheAssoc == 8)
		return 64;
	return 32;
}

int Cache::set_assoc(int cacheAssoc)
{
	int cacheSize = assoc_sizer(cacheAssoc);

	LRU_Entry cache[cacheSize][cacheAssoc] = {0};
	std::vector<std::string>::iterator it;
	int hit = 0;
	for(it = this->list.begin(); it != this->list.end(); it++)
	{
		std::string tmp = *it;
		int addr = std::stoul(tmp.substr(2), nullptr, 16);
		int index = std::floor(addr / 32);
		index %= cacheSize;

		bool found = false;
		for(int i = 0; i < cacheAssoc; i++)
		{
			if(cache[index][i].valid == 1)
			{
				if((cache[index][i].lowBound <= addr) && (cache[index][i].highBound >= addr))
				{
					hit++;
					cache[index][i].timestamp = std::chrono::high_resolution_clock::now();
					found = true;
					break;
				}
			}else if(cache[index][i].valid == 0)
			{
				cache[index][i].lowBound = std::floor(addr / 32) * 32;
				cache[index][i].highBound = cache[index][i].lowBound + 31;
				cache[index][i].valid = 1;
				cache[index][i].timestamp = std::chrono::high_resolution_clock::now();
				found = true;
				break;
			}
		}
		if(!found)
		{
			int oldest = 0;
			for(int i = 1; i < cacheAssoc; i++)
			{
				if(cache[index][oldest].timestamp > cache[index][i].timestamp)
					oldest = i;
			}
			cache[index][oldest].lowBound = std::floor(addr / 32) * 32;
			cache[index][oldest].highBound = cache[index][oldest].lowBound + 31;
			cache[index][oldest].valid = 1;
			cache[index][oldest].timestamp = std::chrono::high_resolution_clock::now();
		}
	}
	return hit;
}

int Cache::full_assoc_LRU()
{
	int cacheSize = 512;
	LRU_Entry cache[cacheSize] = {0};
	std::vector<std::string>::iterator it;
	int hit = 0;
	for(it = this->list.begin(); it != this->list.end(); it++)
	{
		std::string tmp = *it;
		int addr = std::stoul(tmp.substr(2), nullptr, 16);
		
		bool found = false;
		for(int i = 0; i < cacheSize; i++)
		{
			if(cache[i].valid == 1)
			{
				if((cache[i].lowBound <= addr) && (cache[i].highBound >= addr))
				{
					hit++;
					cache[i].timestamp = std::chrono::high_resolution_clock::now();
					found = true;
					break;
				}
			}else
			{
				cache[i].valid = 1;
				cache[i].lowBound = std::floor(addr / 32) * 32;
				cache[i].highBound = cache[i].lowBound + 31;
				cache[i].timestamp = std::chrono::high_resolution_clock::now();
				found = true;
				break;
			}
		}

		if(!found)
		{
			int oldest = 0;
			for(int i = 0; i < cacheSize; i++)
			{
				if(cache[i].timestamp < cache[oldest].timestamp)
					oldest = i;
			}
			cache[oldest].valid = 1;
			cache[oldest].lowBound = std::floor(addr / 32) * 32;
			cache[oldest].highBound = cache[oldest].lowBound + 31;
			cache[oldest].timestamp = std::chrono::high_resolution_clock::now();
		}
	}
	return hit;
}