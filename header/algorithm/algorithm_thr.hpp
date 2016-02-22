#ifndef ALGORITHM_THR
#define ALGORITHM_THR
#include<algorithm>	//for_each, move, remove_if
#include<cstddef>	//size_t
#include<functional>	//bind, equal_to, function, placeholders, ref
#include<future>	//async, launch
#include<iterator>	//distance, iterator_traits, next
#include"algorithm.hpp"

namespace nAlgorithm
{
	//will reorder the permutation of [begin,end)
	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end,const std::size_t N,const BinaryPred pred=BinaryPred())
	{
		using namespace std;
		function<FwdIter(FwdIter,FwdIter)> unique_without_sort_thr_{[&,N,pred](const FwdIter begin,const FwdIter end){
			const auto size{static_cast<size_t>(distance(begin,end))};
			if(N&&N<size)
			{
				const auto mid{next(begin,size/2)};
				const auto lhs_end{async(launch::async,unique_without_sort_thr_,begin,mid).get()};
				auto rhs_end{async(launch::async,unique_without_sort_thr_,mid,end).get()};
				for_each(begin,lhs_end,[&,pred,mid](const typename iterator_traits<FwdIter>::value_type &val){
					rhs_end=remove_if(mid,rhs_end,bind(pred,ref(val),placeholders::_1));
				});
				return move(mid,rhs_end,lhs_end);
			}
			return unique_without_sort(begin,end,pred);
		}};
		return unique_without_sort_thr_(begin,end);
	}

	template<class FwdIter>
	inline FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end)
	{
		return unique_without_sort_thr(begin,end,std::distance(begin,end)/2);
	}
}

#endif