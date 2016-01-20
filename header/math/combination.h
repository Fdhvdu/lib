#ifndef COMBINATION
#define COMBINATION
#include<cstddef>
#include<iterator>
#include<vector>

namespace nMath
{
	template<class T>
	T C(T,T);

	template<class InIter>
	std::vector<std::vector<typename std::iterator_traits<InIter>::value_type>> combination(InIter,InIter,std::size_t);
	//require iterator_traits<InIter>::value_type copyable
}

#include"combination.cpp"

#endif