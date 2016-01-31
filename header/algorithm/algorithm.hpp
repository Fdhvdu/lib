#ifndef ALGORITHM
#define ALGORITHM
#include<algorithm>	//move
#include<cstddef>
#include<functional>	//bind, placeholders
#include<iterator>
#include<type_traits>	//make_unsigned
#include<utility>	//move
#include"../thread/CSmartThread.hpp"

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
		for(InIter iter;(iter=find_if(begin,end,pred))!=end;)
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

	template<class FwdIter,class BinaryPred=std::equal_to<typename std::iterator_traits<FwdIter>::value_type>>
	FwdIter unique_without_sort(FwdIter begin,FwdIter end,const BinaryPred pred=BinaryPred())
	{
		while(begin!=end)
		{
			end=std::remove_if(std::next(begin),end,std::bind(pred,*begin,std::placeholders::_1));
			++begin;
		}
		return end;
	}

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
					rhs_end=std::remove_if(mid,rhs_end,std::bind(pred,val,placeholders::_1));
				});
				divide=std::move(mid,rhs_end,lhs_end);
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
	//Both of unique_without_sort and unique_without_sort_thr will reorder the input permutation.
}

#endif