#ifndef TMP_UTIL
#define TMP_UTIL
#include<cstddef>
#include<stdexcept>
#include<tuple>

namespace nTMP
{
	template<class Ret,std::size_t I,class ... T>
	struct New_obj
	{
		using type=typename std::tuple_element<I,std::tuple<T...>>::type;
		static Ret get(const std::size_t i)
		{
			if(i==I)
				return Ret(new type());
			return New_obj<Ret,I-1,T...>::get(i);
		}
	};

	template<class Ret,class ... T>
	struct New_obj<Ret,0,T...>
	{
		using type=typename std::tuple_element<0,std::tuple<T...>>::type;
		static Ret get(const std::size_t i)
		{
			return Ret(new type());
		}
	};

	template<class Ret,class ... T>
	static Ret get_n_obj(const std::size_t n)
	{
		static constexpr auto size(sizeof...(T));
		if(size==n)
			throw std::out_of_range("n is greater than size");
		return New_obj<Ret,size-1,T...>::get(n);
	}
}

#endif
