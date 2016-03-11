#ifndef EXCHANGE_ENDPOINT_BY_SWAPPING
#define EXCHANGE_ENDPOINT_BY_SWAPPING
#include<algorithm>
#include<functional>
#include<iterator>	//cbegin, cend
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
		using Temp_Container=vector<vector<pair<const BidIter,const BidIter>>>;
		const auto dist{static_cast<size_t>(distance(begin,end))};
		if(dist<2)
			return {};
		function<void(BidIter,BidIter,Temp_Container &,size_t)> exchange_endpoint_by_swapping_;
		const auto reserve{[dist](typename Temp_Container::value_type &container){
			container.reserve(2*dist-3);
		}};
		const function<void(BidIter,BidIter,Temp_Container &,size_t)> to_right_{[&](BidIter to_right,const BidIter to_left,Temp_Container &container,const size_t level){
			container.back().emplace_back(to_right,next(to_right));
			advance(to_right,1);
			if(to_right==to_left)
				exchange_endpoint_by_swapping_(to_right,prev(to_left),container,level+1);
			else
				exchange_endpoint_by_swapping_(to_right,to_left,container,level+1);
		}};
		const function<void(BidIter,BidIter,Temp_Container &,size_t)> to_left_{[&](const BidIter to_right,BidIter to_left,Temp_Container &container,const size_t level){
			container.back().emplace_back(prev(to_left),to_left);
			advance(to_left,-1);
			if(to_right==to_left)
				exchange_endpoint_by_swapping_(next(to_right),to_left,container,level+1);
			else
				exchange_endpoint_by_swapping_(to_right,to_left,container,level+1);
		}};
		exchange_endpoint_by_swapping_=[&,begin,dist,end](const BidIter to_right,const BidIter to_left,Temp_Container &container,const size_t level){
			if(next(to_right)==to_left)
				to_right_(to_right,to_left,container,level);
			else
			{
				bool copy{false};
				if(to_right!=prev(end))
				{
					to_right_(to_right,to_left,container,level);
					copy=true;
				}
				if(to_left!=begin)
				{
					if(copy)
					{
						container.emplace_back(std::cbegin(container.back()),next(std::cbegin(container.back()),level));
						reserve(container.back());
					}
					to_left_(to_right,to_left,container,level);
				}
			}
		};
		Temp_Container container(1);	//not {}
		reserve(container.back());
		exchange_endpoint_by_swapping_(begin,prev(end),container,0);
		result_of_t<decltype(exchange_endpoint_by_swapping<BidIter,BinaryOp>)&(BidIter,BidIter,BinaryOp)> result;
		result.reserve(container.size());
		for(const auto &val:container)
		{
			result.emplace_back();
			result.back().reserve(val.size());
			transform(std::cbegin(val),std::cend(val),back_inserter(result.back()),[op](const pair<const BidIter,const BidIter> &val){return op(*val.first,*val.second);});
		}
		return result;
	}
}

#endif