#ifndef COMBINATION
#define COMBINATION
#include<algorithm>
#include<cstddef>	//size_t
#include<functional>	//function
#include<iterator>	//cbegin, cend, next, prev
#include<numeric>	//accumulate, iota
#include<type_traits>	//result_of_t
#include<vector>
#include"math.hpp"

namespace nMath
{
	template<class T>
	T C(const T n,const T k)
	{
		using namespace std;
		//To speed up, I do not call factorial.
		T start{max(n-k,k)};
		vector<T> numerator(n-start),divisor(n-start);
		iota(cbegin(numerator),cend(numerator),start+1);
		iota(cbegin(divisor),cend(divisor),1);
		T product{1};
		for(auto &val:numerator)
		{
			for(auto &val2:numerator)
			{
				const T temp{gcd(val,val2)};
				if(temp!=1)
				{
					val/=temp;
					val2/=temp;
				}
			}
			product*=val;
		}
		accumulate(cbegin(divisor),cend(divisor),product,[](const auto init,const auto val){return init/val;});
		return product;
	}

	//require iterator_traits<InIter>::value_type copyable
	template<class InIter>
	std::vector<std::vector<typename std::iterator_traits<InIter>::value_type>> combination(InIter gbegin,const InIter gend,const std::size_t take_count)
	{
		using namespace std;
		using Vec=result_of_t<decltype(combination<InIter>)&(InIter,InIter,size_t)>;
		function<void(InIter,InIter,Vec &,size_t)> combination_{[&,gend](InIter begin,const InIter end,Vec &vec,const size_t level){
			while(begin!=end)
			{
				vec.back().emplace_back(*begin);
				if(end!=gend)
					combination_(next(begin),next(end),vec,level+1);
				++begin;
				if(begin!=end)
					vec.emplace_back(std::cbegin(vec.back()),next(std::cbegin(vec.back()),level));
			}
		}};
		const auto dis{static_cast<size_t>(distance(gbegin,gend))};
		if(!dis||dis<take_count)
			return {};
		Vec vec(1);	//not {}
		vec.reserve(C(dis,take_count));
		combination_(gbegin,prev(gend,take_count-1),vec,0);
		return vec;
	}
}

#endif