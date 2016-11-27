#ifndef SHOW
#define SHOW
#include<algorithm>	//for_each
#include<iostream>
#include<iterator>	//cbegin, cend, iterator_traits
#include<utility>	//forward

namespace nTool
{
	template<class InIter,class T,class UnaryOp>
	void show(InIter begin,InIter end,std::ostream &os,const T &delim,UnaryOp &&op)
	{
		using namespace std;
		if(begin!=end)
		{
			os<<std::forward<decltype(op)>(op)(*begin);
			for_each(++begin,end,[&](const typename iterator_traits<InIter>::value_type &val){os<<delim<<std::forward<decltype(op)>(op)(val);});
			os<<endl;
		}
	}

	template<class InIter,class T>
	inline void show(const InIter begin,const InIter end,std::ostream &os,const T &delim)
	{
		show(begin,end,os,delim,[](const typename std::iterator_traits<InIter>::value_type &val) noexcept->const auto&{return val;});
	}

	template<class InIter>
	inline void show(const InIter begin,const InIter end,std::ostream &os=std::cout)
	{
		show(begin,end,os,' ');
	}

	template<class Begin_and_end,class T>
	inline void show(const Begin_and_end &t,std::ostream &os,const T &delim)
	{
		show(std::cbegin(t),std::cend(t),os,delim);
	}

	template<class Begin_and_end>
	inline void show(const Begin_and_end &t,std::ostream &os=std::cout)
	{
		show(t,os,' ');
	}
}

#endif