#ifndef CLOCK_QUEUE
#define CLOCK_QUEUE
#include<mutex>
#include"CBasic_ts_container.hpp"
#include"Lock_queue.hpp"
#include"tag.hpp"

namespace nThread
{
	template<class T,class PopIfExist=Do_not_use_pop_if_exist>
	using CLock_queue=CBasic_ts_container<Lock_queue<T,PopIfExist,std::mutex>>;
}

#endif