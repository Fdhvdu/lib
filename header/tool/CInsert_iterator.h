#ifndef CINSERT_ITERATOR
#define CINSERT_ITERATOR
#include<iterator>

namespace nTool
{
	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	class CInsert_iterator:public std::iterator<std::output_iterator_tag,void,void,void,void>
	{
		Hold *hold_;
		MoveFunc_t move_;
		RefFunc_t ref_;
	public:
		typedef T value_type;
		CInsert_iterator(Hold &,RefFunc_t) noexcept;
		CInsert_iterator(Hold &,RefFunc_t,MoveFunc_t) noexcept;
		inline CInsert_iterator& operator*() noexcept
		{
			return *this;
		}
		inline CInsert_iterator& operator++() noexcept
		{
			return *this;
		}
		inline CInsert_iterator& operator++(int) noexcept
		{
			return *this;
		}
		CInsert_iterator& operator=(const value_type &);
		CInsert_iterator& operator=(value_type &&);
	};

	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	inline CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t> inserter(Hold &hold,const RefFunc_t ref,const MoveFunc_t move)
	{
		return CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>(hold,ref,move);
	}

	template<class T,class Hold,class RefFunc_t>
	inline CInsert_iterator<T,Hold,RefFunc_t,RefFunc_t> inserter(Hold &hold,const RefFunc_t ref)
	{
		return inserter<T>(hold,ref,ref);
	}
}

#include"CInsert_iterator.cpp"

#endif