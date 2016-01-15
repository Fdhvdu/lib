#ifndef EXCHANGE_ENDPOINT_BY_SWAPPING
#define EXCHANGE_ENDPOINT_BY_SWAPPING
#include<iterator>
#include<type_traits>	//result_of
#include<vector>

namespace nAlgorithm
{
	template<class BidIter,class BinaryOp>
	std::vector<std::vector<typename std::result_of<BinaryOp(typename std::iterator_traits<BidIter>::value_type,typename std::iterator_traits<BidIter>::value_type)>::type>> exchange_endpoint_by_swapping(BidIter,BidIter,BinaryOp);
	//vector<vector<return type of BinaryOp>>
}

#include"exchange_endpoint_by_swapping.cpp"

#endif