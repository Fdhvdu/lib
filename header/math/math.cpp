#include"math.h"
#include<stdexcept>	//runtime_error
#include"../algorithm/algorithm.h"

namespace nMath
{
	template<class T>
	T abs(const T val) noexcept
	{
		if(val<0)
			return -1*val;
		return val;
	}

	template<class T>
	T factorial(T n) noexcept
	{
		T temp{1};
		while(n)
			temp*=n--;
		return temp;
	}

	template<class T>
	T gcd(T lhs,T rhs) noexcept
	{
		while(rhs)
		{
			const T temp{lhs%rhs};
			lhs=rhs;
			rhs=temp;
		}
		return lhs;
	}

	template<class T>
	std::size_t log_2(const T val)
	{
		const std::bitset<sizeof(T)*8> temp{val};
		if(temp.count()==1)
			return nAlgorithm::find_if_val<std::size_t>(0,temp.size(),[&](const auto i){return temp[i];});
		throw std::runtime_error{"The argument of log_2 is not a power of 2"};
	}
	
	template<class T,class OutIter>
	void reverse_bit(T val,OutIter begin,const OutIter end)
	{
		while(begin!=end)
		{
			*begin=val&1;
			val>>=1;
			++begin;
		}
	}
}