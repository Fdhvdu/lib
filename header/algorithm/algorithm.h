#ifndef ALGORITHM
#define ALGORITHM
#include<cstddef>
#include<functional>
#include<iterator>

namespace nAlgorithm
{
	template<class T,class UnaryPred>
	bool all_of(T,T,UnaryPred);

	template<class T,class UnaryPred>
	T find_if(T,T,UnaryPred);

	template<class T,class UnaryPred>
	inline bool any_of(const T begin,const T end,UnaryPred pred)
	{
		return find_if(begin,end,pred)!=end;
	}

	template<class T,class UnaryPred>
	std::ptrdiff_t count_if(T,T,UnaryPred);
	
	template<class T,class UnaryFunc>
	UnaryFunc for_each(T,T,UnaryFunc);

	template<class BidIter,class OutIter,class BinaryOp>
	void exchange_front_and_back_with_swap(BidIter,BidIter,OutIter,BinaryOp);

	template<class InIter,class OutIter,class BinaryOp>
	void multiply(InIter,InIter,InIter,InIter,OutIter,BinaryOp);

	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort(FwdIter,FwdIter,BinaryPred pred=BinaryPred());

	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort_thr(FwdIter,FwdIter,std::size_t N,BinaryPred pred=BinaryPred());

	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	inline FwdIter unique_without_sort_thr(FwdIter begin,FwdIter end,BinaryPred pred=BinaryPred())
	{
		return unique_without_sort_thr(begin,end,std::distance(begin,end)/2,pred);
	}
	//Both of unique_without_sort and unique_without_sort_thr will reorder the input permutation.
}

#include"algorithm.cpp"

#endif