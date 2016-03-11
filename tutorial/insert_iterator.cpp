#include<algorithm>
#include<iostream>
#include<iterator>
#include<vector>
#include"../header/tool/CInsert_iterator.hpp"
using namespace std;

struct A
{
	void set(const int &)
	{
		cout<<"const int &"<<endl;
	}
};

struct B
{
	void set(const int &)
	{
		cout<<"const int &"<<endl;
	}
	void set(int &&)
	{
		cout<<"int &&"<<endl;
	}
};

struct C
{
	template<class T>
	void set(T &&)
	{
		cout<<"T &&"<<endl;
	}
};

template<class T>
struct D
{
	void set(const T &)
	{
		cout<<"const T &"<<endl;
	}
};

template<class T>
struct E
{
	void set(const T &)
	{
		cout<<"const T &"<<endl;
	}
	void set(T &&)
	{
		cout<<"T &&"<<endl;
	}
};

template<class T>
struct F
{
	template<class T2>
	void set(T2 &&)
	{
		cout<<"T2 &&"<<endl;
	}
};

int main()
{
	A a;
	B b;
	C c;
	D<int> d;
	E<int> e;
	F<double> f;
	vector<int> vec{1};

	copy(begin(vec),end(vec),nTool::inserter<int,void(A::*)(const int &),&A::set>(a));

	copy(begin(vec),end(vec),nTool::inserter<int,
		 void(B::*)(const int &),&B::set,
		 void(B::*)(int &&),&B::set>(b));

	copy(make_move_iterator(begin(vec)),make_move_iterator(end(vec)),nTool::inserter<int,
		 void(B::*)(const int &),&B::set,
		 void(B::*)(int &&),&B::set>(b));

	copy(begin(vec),end(vec),nTool::inserter<int,
		 void(C::*)(const int &),&C::set<const int &>,
		 void(C::*)(int &&),&C::set<int>>(c));

	copy(begin(vec),end(vec),nTool::inserter<int,
		 void(D<int>::*)(const int &),&D<int>::set>(d));

	copy(begin(vec),end(vec),nTool::inserter<int,
		 void(E<int>::*)(const int &),&E<int>::set,
		 void(E<int>::*)(int &&),&E<int>::set>(e));

	copy(make_move_iterator(begin(vec)),make_move_iterator(end(vec)),nTool::inserter<int,
		 void(E<int>::*)(const int &),&E<int>::set,
		 void(E<int>::*)(int &&),&E<int>::set>(e));

	copy(begin(vec),end(vec),nTool::inserter<int,
		 void(F<double>::*)(const int &),&F<double>::set<const int &>,
		 void(F<double>::*)(int &&),&F<double>::set<int>>(f));
}