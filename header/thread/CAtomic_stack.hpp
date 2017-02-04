#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include"CBasic_ts_container.hpp"
#include"Atomic_stack.hpp"
#include"tag.hpp"

namespace nThread
{
	template<class T>
	using CAtomic_stack=CBasic_ts_container<Atomic_stack<T,Do_not_use_pop_if_exist>>;
}

#endif