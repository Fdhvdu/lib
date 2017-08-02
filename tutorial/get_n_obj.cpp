#include<iostream>
#include"../header/TMP/experimental/TMP_util.hpp"

struct A{virtual ~A()=default;};
struct B:public A{};
struct C:public A{};

void test(const A *p)
{
	using namespace std;
	if(dynamic_cast<const B *>(p))
		cout<<'B'<<endl;
	else
		if(dynamic_cast<const C *>(p))
			cout<<'C'<<endl;
		else
			cout<<'A'<<endl;
}

int main()
{
	test(nTMP::get_n_obj<A*,B,C,A,B>(0));
	test(nTMP::get_n_obj<A*,B,C,A,B>(1));
	test(nTMP::get_n_obj<A*,B,C,A,B>(2));
	test(nTMP::get_n_obj<A*,B,C,A,B>(3));
}
