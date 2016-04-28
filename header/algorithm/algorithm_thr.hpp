#ifndef ALGORITHM_THR
#define ALGORITHM_THR
#include<algorithm>	//for_each, move, remove_if
#include<cstddef>	//size_t
#include<functional>	//equal_to, function
#include<future>	//async, launch
#include<iterator>	//distance, iterator_traits, next
#include<utility>	//forward
#include"algorithm.hpp"

namespace nAlgorithm
{
	//will reorder the permutation of [begin,end)
	//dereferenced FwdIter must meet the requirements of MoveAssignable
	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end,const std::size_t N,const BinaryPred pred=BinaryPred())
	{
		using namespace std;
		const function<FwdIter(FwdIter,FwdIter)> unique_without_sort_thr_{[&,N,pred](const FwdIter begin,const FwdIter end){
			const auto size{static_cast<size_t>(distance(begin,end))};
			if(N&&N<size)
			{
				const auto mid{next(begin,size/2)};
				FwdIter lhs_end,rhs_end;
				{
					future<FwdIter> fut{async(launch::async,unique_without_sort_thr_,begin,mid)};
					rhs_end=async(launch::async,unique_without_sort_thr_,mid,end);
					lhs_end=fut.get();
				}
				for_each(begin,lhs_end,[&,pred,mid](const auto &val){
					rhs_end=remove_if(mid,rhs_end,[&,pred](auto &&val2){
						return pred(val,forward<decltype(val2)>(val2));
					});
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