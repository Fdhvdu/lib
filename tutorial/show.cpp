#include<fstream>
#include<iostream>
#include<string>
#include<vector>
#include"../header/tool/show.h"
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
	nTool::show(vec.begin(),vec.end());

	fstream ofs("output");
	nTool::show(vec.begin(),vec.end(),ofs);
	//write vector message to file

	nTool::show(vec.begin(),vec.end(),cout,',');
	//use ',' as delimiter

	Delim delim{"\nhello world\n"};
	nTool::show(vec.begin(),vec.end(),cout,delim);
	//use "\nhello world\n" as delimiter

	vector<CTest> vec2(5);
	nTool::show(vec2.begin(),vec2.end(),cout,' ',[](const CTest &val){return val.get();});
	//use lambda to tell show how to show the message of CTest
}