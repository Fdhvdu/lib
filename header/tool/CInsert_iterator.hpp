#ifndef CINSERT_ITERATOR
#define CINSERT_ITERATOR
#include<iterator>
#include<memory>	//addressof
#include<utility>	//declval, move

namespace nTool
{
	template<class T,class Holder,class RefFunc,RefFunc ref_func,class MoveFunc=RefFunc,MoveFunc move_func=ref_func>
	class CInsert_iterator:public std::iterator<std::output_iterator_tag,void,void,void,void>
	{
		Holder *hold_;
	public:
		using holder_type=Holder;
		using value_type=T;
		explicit CInsert_iterator(holder_type &hold) noexcept
			:hold_(std::addressof(hold)){}
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
		CInsert_iterator& operator=(const value_type &val) noexcept(noexcept((std::declval<holder_type>().*ref_func)(std::declval<value_type&>())))
		{
			(hold_->*ref_func)(val);
			return *this;
		}
		CInsert_iterator& operator=(value_type &&val) noexcept(noexcept((std::declval<holder_type>().*move_func)(std::declval<value_type>())))
		{
			(hold_->*move_func)(std::move(val));
			return *this;
		}
	};

	template<class T,class RefFunc,RefFunc ref_func,class MoveFunc,MoveFunc move_func,class Holder>
	inline CInsert_iterator<T,Holder,RefFunc,ref_func,MoveFunc,move_func> inserter(Holder &holder) noexcept
	{
		return CInsert_iterator<T,Holder,RefFunc,ref_func,MoveFunc,move_func>{holder};
	}

	template<class T,class RefFunc,RefFunc ref_func,class Holder>
	inline CInsert_iterator<T,Holder,RefFunc,ref_func> inserter(Holder &holder) noexcept
	{
		return CInsert_iterator<T,Holder,RefFunc,ref_func>{holder};
	}
}

#endif
