#include"CInsert_iterator.h"
#include<type_traits>
#include<utility>

namespace nTool
{
	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>::CInsert_iterator(Hold &hold,const RefFunc_t ref) noexcept
		:CInsert_iterator{hold,ref,ref}{}

	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>::CInsert_iterator(Hold &hold,const RefFunc_t ref,const MoveFunc_t move) noexcept
		:hold_{std::addressof(hold)},move_{move},ref_{ref}{}

	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>& CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>::operator=(const value_type &val)
	{
		(hold_->*ref_)(val);
		return *this;
	}

	template<class T,class Hold,class RefFunc_t,class MoveFunc_t>
	CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>& CInsert_iterator<T,Hold,RefFunc_t,MoveFunc_t>::operator=(value_type &&rVal)
	{
		(hold_->*move_)(std::move(rVal));
		return *this;
	}
}