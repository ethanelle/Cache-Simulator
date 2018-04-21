#ifndef CACHE
#define CACHE

#include <string>
#include <vector>
#include <fstream>

class Cache{
private:
	std::vector<std::string> list;
	std::fstream outFile;
	int direct_map(int);
	int set_assoc(int);
	int assoc_sizer(int);
	int full_assoc_LRU();
public:
	Cache(std::vector<std::string>, std::string);
	~Cache();
	void run_sim();
};

#endif