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

	template<class InIter,class T,class BinaryPred>
	InIter find_val_if(InIter begin,const InIter end,const T &val,const BinaryPred pred)
	{
		while(begin!=end)
		{
			if(pred(*begin,val))
				return begin;
			++begin;
		}
		return begin;
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
	void multiply(InIter lbegin,const InIter lend,const InIter rbegin,const InIter rend,OutIter des,const BinaryOp op)
	{
		while(lbegin!=lend)
		{
			for(auto begin{rbegin};begin!=rend;++begin,++des)
				*des=op(*lbegin,*begin);
			++lbegin;
		}
	}

	template<class FwdIter,class T,class BinaryPred>
	FwdIter remove_val_if(FwdIter begin,const FwdIter end,const T &val,const BinaryPred pred)
	{
		begin=find_val_if(begin,end,val,pred);
		if(begin!=end)
			for(auto temp{begin};++temp!=end;)
				if(!pred(*temp,val))
					*begin++=std::move(*temp);
		return begin;
	}

	template<class FwdIter,class BinaryPred>
	FwdIter unique_without_sort(FwdIter begin,FwdIter end,const BinaryPred pred)
	{
		while(begin!=end)
		{
			end=remove_val_if(std::next(begin),end,*begin,pred);
			++begin;
		}
		return end;
	}

	template<class FwdIter,class BinaryPred>
	FwdIter unique_without_sort_thr(const FwdIter begin,const FwdIter end,const std::size_t N,const BinaryPred pred)
	{
		using namespace std;
		function<void(FwdIter,FwdIter,FwdIter &)> unique_without_sort_thr_{[&,N,pred](const FwdIter begin,const FwdIter end,FwdIter &divide){
			const size_t size{make_unsigned<ptrdiff_t>::type(distance(begin,end))};
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
					rhs_end=remove_val_if(mid,rhs_end,val,pred);
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
}