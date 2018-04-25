#include "cache.h"
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <ctime>
#include <chrono>

#define DIRECTMAP 1
#define SETASSOC 1
#define FULLYASSOC 1
#define WRITEMISS 1
#define PREFETCH 1
#define PREFETCHMISS 1
#define EXTRA 0

struct LRU_Entry {
	int valid;
	int lowBound;
	int highBound;
	std::chrono::high_resolution_clock::time_point timestamp;
};

struct HC_Entry {
	int valid;
	int lowBound;
	int highBound;
	int temperature;
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
	int cacheSize, cacheAssoc;

#if DIRECTMAP
	std::cout << "ACTUAL:\t\t";
	cacheSize = 32;
	while(cacheSize <= 1024)
	{
		hit = direct_map(cacheSize);
		std::cout << hit << "," << total << "; ";
		this->outFile << hit << "," << total << "; ";

		if(cacheSize == 32)
			cacheSize = 128;
		else if(cacheSize == 128)
			cacheSize = 512;
		else if(cacheSize == 512)
			cacheSize = 1024;
		else
		{
			std::cout << std::endl;
			this->outFile << std::endl;
			break;
		}
	}
	std::cout << "DESIRED:\t837589,1122102; 932528,1122102; 972661,1122102; 1005547,1122102;" << std::endl;
#endif
#if SETASSOC
	std::cout << "\nACTUAL:\t\t";
	cacheAssoc = 2;
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
#if FULLYASSOC
	std::cout << "\nACTUAL:\t\t";
	hit = full_assoc_LRU();
	std::cout << hit << "," << total << ";" << std::endl;
	this->outFile << hit << "," << total << ";" << std::endl;;
	std::cout << "DESIRED:\t1000500,1122102;" << std::endl;

	std::cout << "\nACTUAL:\t\t";
	hit = full_assoc_HC();
	std::cout << hit << "," << total << ";" << std::endl;
	this->outFile << hit << "," << total << ";" << std::endl;
	std::cout << "DESIRED:\t998466,1122102;" << std::endl;
#endif
#if WRITEMISS
	std::cout << "\nACTUAL:\t\t";
	cacheAssoc = 2;
	while(cacheAssoc <= 16)
	{
		hit = set_assoc_write_miss(cacheAssoc);
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
	std::cout << "DESIRED:\t991430,1122102; 996265,1122102; 996942,1122102; 997995,1122102;" << std::endl;
#endif
#if PREFETCH
	std::cout << "\nACTUAL:\t\t";
	cacheAssoc = 2;
	while(cacheAssoc <= 16)
	{
		hit = set_assoc_prefetch(cacheAssoc);
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
	std::cout << "DESIRED:\t996682,1122102; 1001716,1122102; 1002738,1122102; 1003417,1122102;" << std::endl;
#endif
#if PREFETCHMISS
	std::cout << "\nACTUAL:\t\t";
	cacheAssoc = 2;
	while(cacheAssoc <= 16)
	{
		hit = set_assoc_prefetch_miss(cacheAssoc);
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
	std::cout << "DESIRED:\t992669,1122102; 998384,1122102; 999371,1122102; 1000391,1122102;" << std::endl;
#endif
#if EXTRA
	hit = set_assoc_shallow();
	std::cout << hit << "," << total << ";" << std::endl;
	this->outFile << hit << "," << total << ";" << std::endl;
#endif
}

int Cache::direct_map(int cacheSize)
{
	LRU_Entry cache[cacheSize] = {0};
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
			}
		}else
		{
			cache[index].valid = 1;
			cache[index].lowBound = std::floor(addr / 32) * 32;
			cache[index].highBound = cache[index].lowBound + 31;
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

int Cache::full_assoc_HC()
{
	int cacheSize = 512;
	HC_Entry cache[cacheSize] = {0};
//	int HC[cacheSize-1] = {0};
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
					cache[i].temperature += 2;
					found = true;
					break;
				}
			}else
			{
				cache[i].valid = 1;
				cache[i].lowBound = std::floor(addr / 32) * 32;
				cache[i].highBound = cache[i].lowBound + 31;
				cache[i].temperature = 1;
				found = true;
				break;
			}
		}

		if(!found) //evict based on entry with "lowest" temperature
		{
			int coldest = 0;
			for(int i = 0; i < cacheSize; i++)
			{
				if(cache[coldest].temperature > cache[i].temperature)
					coldest = i;
			}
			cache[coldest].valid = 1;
			cache[coldest].lowBound = std::floor(addr / 32) * 32;
			cache[coldest].highBound = cache[coldest].lowBound + 31;
			cache[coldest].temperature = 1;
		}
	}
	return hit;
}

int Cache::set_assoc_write_miss(int cacheAssoc)
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
				if(tmp.substr(0,1).compare("S") != 0)
				{
					cache[index][i].lowBound = std::floor(addr / 32) * 32;
					cache[index][i].highBound = cache[index][i].lowBound + 31;
					cache[index][i].valid = 1;
					cache[index][i].timestamp = std::chrono::high_resolution_clock::now();
				}
				found = true;
				break;
			}
		}
		if((!found) && (tmp.substr(0,1).compare("S") != 0))
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

int Cache::set_assoc_prefetch(int cacheAssoc)
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

		/* PREFETCH */
		addr += 32;
		index = std::floor(addr / 32);
		index %= cacheSize;

		found = false;
		for(int i = 0; i < cacheAssoc; i++)
		{
			if(cache[index][i].valid == 1)
			{
				if((cache[index][i].lowBound <= addr) && (cache[index][i].highBound >= addr))
				{
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

int Cache::set_assoc_prefetch_miss(int cacheAssoc)
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
		bool miss = false;
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
				miss = true;
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
			miss = true;
		}

		/* PREFECT */
		if(miss)
		{
			addr += 32;
			index = std::floor(addr / 32);
			index %= cacheSize;

			found = false;
			for(int i = 0; i < cacheAssoc; i++)
			{
				if(cache[index][i].valid == 1)
				{
					if((cache[index][i].lowBound <= addr) && (cache[index][i].highBound >= addr))
					{
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
	}
	return hit;
}

int Cache::set_assoc_shallow()
{
	
}