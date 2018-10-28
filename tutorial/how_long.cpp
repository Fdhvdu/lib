#include<cstdint>
#include<iostream>
#include<limits>
#include"../header/tool/how_long.hpp"
using namespace std;

int main()
{
	cout<<"it takes "<<chrono::duration_cast<chrono::seconds>(nTool::how_long([]{
		for(volatile auto i(numeric_limits<uintmax_t>::max());--i;)
			;
	})).count()<<" seconds"<<endl;
}