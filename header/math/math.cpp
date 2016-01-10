#include"math.h"
#include<algorithm>
#include<functional>
#include<iterator>
#include<numeric>
#include<stdexcept>	//runtime_error
#include<vector>
#include"../algorithm/algorithm.h"

namespace nMath
{
	template<class T>
	T abs(const T val)
	{
		if(val<0)
			return -1*val;
		return val;
	}

	template<class T>
	T C(const T n,const T k)
	{
		using namespace std;
		//To speed up, I do not call factorial.
		T start{max(n-k,k)};
		vector<T> numerator(n-start),divisor(n-start);
		iota(numerator.begin(),numerator.end(),start+1);
		iota(divisor.begin(),divisor.end(),1);
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
		for(auto val:divisor)
			product/=val;
		return product;
	}

	template<class InIter,class OutIter,class UnaryOp>
	void combination(InIter begin,const InIter end,const std::size_t takeCount,OutIter des,const UnaryOp pred)
	{
		using namespace std;
		OutIter &desEnd{des};
		function<void(InIter,OutIter,const size_t)> combination={[&,takeCount,pred](InIter begin,OutIter des,const size_t level){
			if(takeCount!=level)
				while(begin!=end)
				{
					combination(next(begin),des,level+1);
					while(des!=desEnd)
					{
						*next(pred(*des),level)=*begin;
						++des;
					}
					++begin;
				}
			else
				++desEnd;
		}};
		combination(begin,des,0);
	}

	template<class T>
	T factorial(T n)
	{
		T temp{1};
		while(n)
			temp*=n--;
		return temp;
	}

	template<class T>
	T gcd(T lhs,T rhs)
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
			return nAlgorithm::find_if<std::size_t>(0,temp.size(),[&](const auto i){return temp[i];});
		throw std::runtime_error{"the argument of log_2 is not a power of 2"};
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