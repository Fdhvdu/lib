#ifndef CINSERT_ITERATOR
#define CINSERT_ITERATOR
#include<iterator>
#include<memory>	//addressof
#include<utility>	//move

namespace nTool
{
	template<class T,class Hold,class RefFunc,class MoveFunc>
	class CInsert_iterator:public std::iterator<std::output_iterator_tag,void,void,void,void>
	{
		Hold *hold_;
		MoveFunc move_;
		RefFunc ref_;
	public:
		typedef T value_type;
		CInsert_iterator(Hold &hold,const RefFunc ref) noexcept
			:CInsert_iterator{hold,ref,ref}{}
		CInsert_iterator(Hold &hold,const RefFunc ref,const MoveFunc move) noexcept
			:hold_{std::addressof(hold)},move_{move},ref_{ref}{}
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
		CInsert_iterator& operator=(const value_type &val)
		{
			(hold_->*ref_)(val);
			return *this;
		}
		CInsert_iterator& operator=(value_type &&xval)
		{
			(hold_->*move_)(std::move(xval));
			return *this;
		}
	};

	template<class T,class Hold,class RefFunc,class MoveFunc>
	inline CInsert_iterator<T,Hold,RefFunc,MoveFunc> inserter(Hold &hold,const RefFunc ref,const MoveFunc move)
	{
		return CInsert_iterator<T,Hold,RefFunc,MoveFunc>{hold,ref,move};
	}

	template<class T,class Hold,class RefFunc>
	inline CInsert_iterator<T,Hold,RefFunc,RefFunc> inserter(Hold &hold,const RefFunc ref)
	{
		return inserter<T>(hold,ref,ref);
	}
}

#endif