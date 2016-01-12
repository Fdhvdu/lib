#include<algorithm>
#include<iterator>
#include<vector>
#include"../header/tool/CInsert_iterator.h"

struct A
{
	void set(const int &){}
};

struct B
{
	void set(const int &){}
	void set(int &&){}
};

struct C
{
	template<class T>
	void set(T &&){}
};

template<class T>
struct D
{
	void set(const T &){}
};

template<class T>
struct E
{
	void set(const T &){}
	void set(T &&){}
};

template<class T>
struct F
{
	template<class T2>
	void set(T2 &&){}
};

template<class T,class Iter,class Hold,class RefFunc_t,class MoveFunc_t>
inline void test(const Iter begin,const Iter end,Hold &hold,const RefFunc_t ref,const MoveFunc_t mov)
{
	std::copy(begin,end,nTool::inserter<T>(hold,ref,mov));
	//if Iter is move_iterator, nTool::inserter will call move assignment operator
}

int main()
{
	using namespace std;

	A a;
	B b;
	C c;
	D<int> d;
	E<int> e;
	F<double> f;
	vector<int> vec;

	test<int>(begin(vec),end(vec),a,&A::set,&A::set);
	test<int>(begin(vec),end(vec),b,
			  static_cast<void(B::*)(const int &)>(&B::set),
			  static_cast<void(B::*)(int &&)>(&B::set));
	test<int>(begin(vec),end(vec),c,
			  static_cast<void(C::*)(const int &)>(&C::set<const int &>),
			  static_cast<void(C::*)(int &&)>(&C::set<int>));
	test<int>(begin(vec),end(vec),d,&D<int>::set,&D<int>::set);
	test<int>(begin(vec),end(vec),e,
			  static_cast<void(E<int>::*)(const int &)>(&E<int>::set),
			  static_cast<void(E<int>::*)(int &&)>(&E<int>::set));
	test<int>(begin(vec),end(vec),f,
			  static_cast<void(F<double>::*)(const int &)>(&F<double>::set<const int &>),
			  static_cast<void(F<double>::*)(int &&)>(&F<double>::set<int>));
}