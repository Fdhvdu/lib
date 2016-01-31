#ifndef ALGORITHM
#define ALGORITHM
#include<algorithm>	//find_if, remove_if
#include<cstddef>	//ptrdiff_t
#include<functional>	//bind, equal_to, placeholders, ref
#include<iterator>	//iterator_traits, next

namespace nAlgorithm
{
	template<class T,class UnaryPred>
	bool all_of_val(T begin,const T end,const UnaryPred pred)
	{
		while(begin!=end)
		{
			if(!pred(begin))
				return false;
			++begin;
		}
		return true;
	}

	template<class T,class UnaryPred>
	T find_if_val(T begin,const T end,const UnaryPred pred)
	{
		while(begin!=end)
		{
			if(pred(begin))
				return begin;
			++begin;
		}
		return end;
	}

	template<class T,class UnaryPred>
	inline bool any_of_val(const T begin,const T end,const UnaryPred pred)
	{
		return find_if_val(begin,end,pred)!=end;
	}

	template<class T,class UnaryPred>
	std::ptrdiff_t count_if_val(T begin,const T end,const UnaryPred pred)
	{
		std::ptrdiff_t accu{0};
		while(begin!=end)
		{
			if(pred(begin))
				++accu;
			++begin;
		}
		return accu;
	}

	template<class T,class UnaryFunc>
	UnaryFunc for_each_val(T begin,const T end,const UnaryFunc func)
	{
		while(begin!=end)
		{
			func(begin);
			++begin;
		}
		return func;
	}

	template<class InIter,class UnaryPred,class UnaryFunc>
	void loop_until_none_of(const InIter begin,const InIter end,const UnaryPred pred,const UnaryFunc func)
	{
		for(InIter iter;(iter=std::find_if(begin,end,pred))!=end;)
			func(*iter);
	}

	template<class InIter,class OutIter,class BinaryOp>
	void multiply(InIter lbegin,const InIter lend,const InIter rbegin,const InIter rend,OutIter des,const BinaryOp op)
	{
		while(lbegin!=lend)
		{
			for(auto begin{rbegin};begin!=rend;++begin,++des)
				*des=op(*lbegin,*begin);
			++lbegin;
		}
	}

	//unique_without_sort will reorder the input permutation.
	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort(FwdIter begin,FwdIter end,const BinaryPred pred=BinaryPred())
	{
		while(begin!=end)
		{
			end=std::remove_if(std::next(begin),end,std::bind(pred,std::ref(*begin),std::placeholders::_1));
			++begin;
		}
		return end;
	}
}

#endif