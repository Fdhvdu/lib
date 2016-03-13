#ifndef CINSERT_ITERATOR
#define CINSERT_ITERATOR
#include<iterator>
#include<memory>	//addressof
#include<utility>	//declval, move

namespace nTool
{
	template<class T,class Holder,class RefFunc_t,RefFunc_t RefFunc,class MoveFunc_t=RefFunc_t,MoveFunc_t MoveFunc=RefFunc>
	class CInsert_iterator:public std::iterator<std::output_iterator_tag,void,void,void,void>
	{
		Holder *hold_;
	public:
		using holder_type=Holder;
		using value_type=T;
		explicit CInsert_iterator(holder_type &hold) noexcept
			:hold_{std::addressof(hold)}{}
		inline CInsert_iterator& operator*() noexcept
		{
			return *this;
		}
		inline CInsert_iterator& operator++() noexcept
		{
			return *this;
		}
		inline CInsert_iterator& operator++(const int) noexcept
		{
			return *this;
		}
		CInsert_iterator& operator=(const value_type &val) noexcept(noexcept((std::declval<holder_type>().*RefFunc)(std::declval<value_type&>())))
		{
			(hold_->*RefFunc)(val);
			return *this;
		}
		CInsert_iterator& operator=(value_type &&val) noexcept(noexcept((std::declval<holder_type>().*MoveFunc)(std::declval<value_type>())))
		{
			(hold_->*MoveFunc)(std::move(val));
			return *this;
		}
	};

	template<class T,class RefFunc_t,RefFunc_t RefFunc,class MoveFunc_t,MoveFunc_t MoveFunc,class Holder>
	inline CInsert_iterator<T,Holder,RefFunc_t,RefFunc,MoveFunc_t,MoveFunc> inserter(Holder &holder)
	{
		return CInsert_iterator<T,Holder,RefFunc_t,RefFunc,MoveFunc_t,MoveFunc>{holder};
	}

	template<class T,class RefFunc_t,RefFunc_t RefFunc,class Holder>
	inline CInsert_iterator<T,Holder,RefFunc_t,RefFunc> inserter(Holder &holder)
	{
		return CInsert_iterator<T,Holder,RefFunc_t,RefFunc>{holder};
	}
}

#endif