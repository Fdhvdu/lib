#ifndef ALGORITHM_THR
#define ALGORITHM_THR
#include<algorithm>	//move, remove_if
#include<cstddef>	//ptrdiff_t
#include<functional>	//bind, equal_to, function, placeholders, ref
#include<iterator>	//distance, iterator_traits, next
#include<type_traits>	//make_unsigned
#include"algorithm.hpp"
#include"../thread/CSmartThread.hpp"

namespace nAlgorithm
{
	//unique_without_sort_thr will reorder the input permutation.
	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end,const std::size_t N,const BinaryPred pred=BinaryPred())
	{
		using namespace std;
		function<void(FwdIter,FwdIter,FwdIter &)> unique_without_sort_thr_{[&,N,pred](const FwdIter begin,const FwdIter end,FwdIter &divide){
			const size_t size{static_cast<make_unsigned<ptrdiff_t>::type>(distance(begin,end))};
			if(N&&N<size)
			{
				divide=end;
				FwdIter lhs_end,rhs_end;
				const auto mid{next(begin,size/2)};
				{
					nThread::CSmartThread lhs{unique_without_sort_thr_,begin,mid,ref(lhs_end)},
						rhs{unique_without_sort_thr_,mid,end,ref(rhs_end)};
				}
				for_each(begin,lhs_end,[&,pred,mid](const typename iterator_traits<FwdIter>::value_type &val){
					rhs_end=remove_if(mid,rhs_end,bind(pred,val,placeholders::_1));
				});
				divide=move(mid,rhs_end,lhs_end);
			}
			else
				divide=unique_without_sort(begin,end,pred);
		}};
		FwdIter retVal;
		unique_without_sort_thr_(begin,end,retVal);
		return retVal;
	}

	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	inline FwdIter unique_without_sort_thr(FwdIter begin,FwdIter end,BinaryPred pred=BinaryPred())
	{
		return unique_without_sort_thr(begin,end,std::distance(begin,end)/2,pred);
	}
}

#endif