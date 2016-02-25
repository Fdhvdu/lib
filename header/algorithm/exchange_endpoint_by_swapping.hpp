#ifndef EXCHANGE_ENDPOINT_BY_SWAPPING
#define EXCHANGE_ENDPOINT_BY_SWAPPING
#include<algorithm>
#include<functional>
#include<iterator>
#include<type_traits>	//result_of_t
#include<utility>
#include<vector>

namespace nAlgorithm
{
	//vector<vector<return type of BinaryOp>>
	template<class BidIter,class BinaryOp>
	std::vector<std::vector<std::result_of_t<BinaryOp(typename std::iterator_traits<BidIter>::value_type,typename std::iterator_traits<BidIter>::value_type)>>> exchange_endpoint_by_swapping(const BidIter begin,const BidIter end,const BinaryOp op)
	{
		using namespace std;
		using Vec=vector<vector<pair<const BidIter,const BidIter>>>;
		function<void(BidIter,BidIter,Vec &,size_t)> exchange_endpoint_by_swapping_;
		function<void(BidIter,BidIter,Vec &,size_t)> to_right_{[&](BidIter to_right,const BidIter to_left,Vec &vec,const size_t level){
			vec.back().emplace_back(to_right,next(to_right));
			advance(to_right,1);
			if(to_right==to_left)
				exchange_endpoint_by_swapping_(to_right,prev(to_left),vec,level+1);
			else
				exchange_endpoint_by_swapping_(to_right,to_left,vec,level+1);
		}};
		function<void(BidIter,BidIter,Vec &,size_t)> to_left_{[&](const BidIter to_right,BidIter to_left,Vec &vec,const size_t level){
			vec.back().emplace_back(prev(to_left),to_left);
			advance(to_left,-1);
			if(to_right==to_left)
				exchange_endpoint_by_swapping_(next(to_right),to_left,vec,level+1);
			else
				exchange_endpoint_by_swapping_(to_right,to_left,vec,level+1);
		}};
		exchange_endpoint_by_swapping_=[&,begin,end](const BidIter to_right,const BidIter to_left,Vec &vec,const size_t level){
			if(next(to_right)==to_left)
				to_right_(to_right,to_left,vec,level);
			else
			{
				bool copy{false};
				if(to_right!=prev(end))
				{
					to_right_(to_right,to_left,vec,level);
					copy=true;
				}
				if(to_left!=begin)
				{
					if(copy)
						vec.emplace_back(std::begin(vec.back()),next(std::begin(vec.back()),level));
					to_left_(to_right,to_left,vec,level);
				}
			}
		};
		if(static_cast<size_t>(distance(begin,end))<2)
			return {};
		Vec vec(1);	//not {}
		exchange_endpoint_by_swapping_(begin,prev(end),vec,0);
		result_of_t<decltype(exchange_endpoint_by_swapping<BidIter,BinaryOp>)&(BidIter,BidIter,BinaryOp)> result;
		result.reserve(vec.size());
		for(auto &val:vec)
		{
			result.emplace_back();
			result.reserve(val.size());
			transform(std::begin(val),std::end(val),back_inserter(result.back()),[op](const pair<const BidIter,const BidIter> &val){return op(*val.first,*val.second);});
		}
		return result;
	}
}

#endif