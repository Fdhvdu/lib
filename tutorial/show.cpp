#include<fstream>
#include<iostream>
#include<iterator>
#include<string>
#include<vector>
#include"../header/tool/show.hpp"
using namespace std;

class CTest
{
	static int id;
	int i;
public:
	CTest() noexcept;
	inline int get() const noexcept
	{
		return i;
	}
};

struct Delim
{
	string str;
};

CTest::CTest() noexcept
	:i(++id){}

int CTest::id(0);

inline ostream& operator<<(ostream &os,const Delim &delim)
{
	return os<<delim.str;
}

int main()
{
	vector<int> vec{1,2,3,4,5};
	nTool::show(vec);
	nTool::show(next(begin(vec)),end(vec));

	fstream ofs("output");
	nTool::show(begin(vec),end(vec),ofs);
	//write vector message to file

	nTool::show(begin(vec),end(vec),cout,',');
	//use ',' as delimiter

	Delim delim{"\nhello world\n"};
	nTool::show(begin(vec),end(vec),cout,delim);
	//use "\nhello world\n" as delimiter

	vector<CTest> vec2(5);
	nTool::show(begin(vec2),end(vec2),cout,' ',[](const CTest &val){return val.get();});
	//use lambda to tell show how to show the message of CTest
}