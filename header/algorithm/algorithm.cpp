#include"algorithm.h"
#include<algorithm>
#include<utility>
#include"../thread/CSmartThread.h"

namespace nAlgorithm
{
	template<class T,class UnaryPred>
	bool all_of(T begin,const T end,const UnaryPred pred)
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
	std::ptrdiff_t count_if(T begin,const T end,const UnaryPred pred)
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

	template<class T,class UnaryPred>
	T find_if(T begin,const T end,const UnaryPred pred)
	{
		while(begin!=end)
		{
			if(pred(begin))
				return begin;
			++begin;
		}
		return end;
	}

	template<class T,class UnaryFunc>
	UnaryFunc for_each(T begin,const T end,const UnaryFunc func)
	{
		while(begin!=end)
		{
			func(begin);
			++begin;
		}
		return func;
	}

	template<class InIter,class OutIter,class BinaryOp>
	void multiply(InIter lbegin,const InIter lend,const InIter rbegin,const InIter rend,OutIter des,const BinaryOp pred)
	{
		while(lbegin!=lend)
		{
			for(auto begin{rbegin};begin!=rend;++begin)
				*des=pred(*lbegin,*begin);
			++lbegin;
		}
	}

	template<class FwdIter,class BinaryPred>
	FwdIter unique_without_sort(FwdIter begin,FwdIter end,const BinaryPred pred)
	{
		while(begin!=end)
		{
			end=std::remove_if(std::next(begin),end,[&,pred](auto &val){return pred(*begin,std::forward<typename std::iterator_traits<FwdIter>::reference>(val));});
			++begin;
		}
		return end;
	}

	template<class FwdIter,class BinaryPred>
	FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end,const std::size_t N,const BinaryPred pred)
	{
		using namespace std;
		function<void(FwdIter,FwdIter,FwdIter &)> unique_without_sort_thr_{[&,N,pred](FwdIter begin,FwdIter end,FwdIter &divide){
			const size_t size{make_unsigned<ptrdiff_t>::type(distance(begin,end))};
			if(N&&N<size)
			{
				divide=end;
				FwdIter lhsEnd,rhsEnd;
				const auto mid{next(begin,size/2)};
				{
					nThread::CSmartThread lhs{unique_without_sort_thr_,begin,mid,ref(lhsEnd)},
						rhs{unique_without_sort_thr_,mid,end,ref(rhsEnd)};
				}
				while(begin!=lhsEnd)
				{
					rhsEnd=std::remove_if(mid,rhsEnd,[&,pred](auto &val){return pred(*begin,std::forward<typename std::iterator_traits<FwdIter>::reference>(val));});
					++begin;
				}
				divide=std::move(mid,rhsEnd,lhsEnd);
			}
			else
				divide=unique_without_sort(begin,end,pred);
		}};
		FwdIter retVal;
		unique_without_sort_thr_(begin,end,retVal);
		return retVal;
	}
}