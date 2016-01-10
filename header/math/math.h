#ifndef MATH
#define MATH
#include<bitset>
#include<cstddef>

namespace nMath
{
	template<class T>
	T abs(T);

	template<class T>
	T C(T,T);

	template<class InIter,class OutIter,class UnaryOp>
	void combination(InIter,InIter,std::size_t,OutIter,UnaryOp);

	inline std::size_t diff_bit_count(const unsigned long long lhs,const unsigned long long rhs) noexcept
	{
		return std::bitset<sizeof(unsigned long long)*8>{lhs^rhs}.count();
	}

	template<class T>
	T factorial(T);

	template<class T>
	T gcd(T,T);

	template<class T>
	inline bool is_power_of_2(const T val)
	{
		return std::bitset<sizeof(T)*8>{val}.count()==1;
	}

	template<class T>
	std::size_t log_2(T);

	template<class T>
	constexpr std::size_t power_of_2(const T val)
	{
		return static_cast<std::size_t>(1)<<val;
	}
	
	template<class T,class OutIter>
	void reverse_bit(T,OutIter,OutIter);
}

#include"math.cpp"

#endif