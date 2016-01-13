#include"combination.h"
#include<functional>	//function
#include<type_traits>	//make_unsigned
#include"math.h"

namespace nMath
{
	template<class InIter>
	std::vector<std::vector<typename std::iterator_traits<InIter>::value_type>> combination(InIter gbegin,const InIter gend,const std::size_t take_count)
	{
		using namespace std;
		typedef vector<vector<typename iterator_traits<InIter>::value_type>> Vec;
		function<void(InIter,InIter,Vec &,size_t)> combination_{[&,gend](InIter begin,const InIter end,Vec &vec,const size_t level){
			while(begin!=end)
			{
				vec.back().emplace_back(*begin);
				if(end!=gend)
					combination_(next(begin),next(end),vec,level+1);
				++begin;
				if(begin!=end)
					vec.emplace_back(vec.back().begin(),next(vec.back().begin(),level));
			}
		}};
		const auto dis{make_unsigned<ptrdiff_t>::type(distance(gbegin,gend))};
		if(!dis||dis<take_count)
			return {};
		Vec vec(1);
		vec.reserve(C<make_unsigned<ptrdiff_t>::type>(dis,take_count));
		combination_(gbegin,prev(gend,take_count-1),vec,0);
		return vec;
	}
}