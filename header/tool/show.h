#ifndef SHOW
#define SHOW
#include<iostream>
#include<iterator>

namespace nTool
{
	template<class InIter,class T,class UnaryOp>
	void show(InIter,InIter,std::ostream &,const T &,UnaryOp);

	template<class InIter,class T>
	inline void show(InIter begin,InIter end,std::ostream &os,const T &delim)
	{
		show(begin,end,os,delim,[](const typename std::iterator_traits<InIter>::value_type &val)->auto&{return val;});
	}

	template<class InIter>
	inline void show(InIter begin,InIter end,std::ostream &os=std::cout)
	{
		show(begin,end,os,' ');
	}
}

#include"show.cpp"

#endif