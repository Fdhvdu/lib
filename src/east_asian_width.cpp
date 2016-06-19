#include"../header/tool/east_asian_width.hpp"
#include<fstream>
using namespace std;

namespace
{
	unordered_map<unsigned long,char> read_east_asian_width_property()
	{
		ifstream ifs{"east_asian_width_property",ios_base::binary|ios_base::in};
		if(ifs.is_open())
		{
			unordered_map<unsigned long,char> east_asian_width_property{0x110000};
			char c;
			for(unsigned long i{0};ifs>>c;++i)
				east_asian_width_property[i]=c;
			return east_asian_width_property;
		}
		throw runtime_error{"east_asian_width_property does not exist"};
	}
}

namespace nTool
{
	const unordered_map<unsigned long,char> east_asian_width_property{read_east_asian_width_property()};
}