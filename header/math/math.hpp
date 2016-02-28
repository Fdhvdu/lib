#ifndef MATH
#define MATH
#include<bitset>
#include<cstddef>
#include<stdexcept>	//runtime_error
#include"../algorithm/algorithm.hpp"

namespace nMath
{
	template<class T>
	T abs(const T val) noexcept
	{
		if(val<0)
			return -1*val;
		return val;
	}

	constexpr std::size_t Cantor_pairing_function(const std::size_t N1,const std::size_t N2) noexcept	//invented by Georg Ferdinand Ludwig Philipp Cantor
	{
		return (N1+N2)*(N1+N2+1)/2+N2;
	}

	inline std::size_t diff_bit_count(const unsigned long long lhs,const unsigned long long rhs) noexcept
	{
		return std::bitset<sizeof(unsigned long long)*8>{lhs^rhs}.count();
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
	inline bool is_power_of_2(const T val) noexcept
	{
		return std::bitset<sizeof(T)*8>{val}.count()==1;
	}

	template<class T>
	std::size_t log_2(const T val)
	{
		const std::bitset<sizeof(T)*8> temp{val};
		if(temp.count()==1)
			return nAlgorithm::find_if_val<std::size_t>(0,temp.size(),[&](const auto i) noexcept{return temp[i];});
		throw std::runtime_error{"The argument of log_2 is not a power of 2"};
	}

	template<class T>
	constexpr std::size_t power_of_2(const T val) noexcept
	{
		return static_cast<std::size_t>(1)<<val;
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

#endif