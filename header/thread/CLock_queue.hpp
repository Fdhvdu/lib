#ifndef CLOCK_QUEUE
#define CLOCK_QUEUE
#include<mutex>
#include"CBasic_ts_container.hpp"
#include"Lock_queue.hpp"

namespace nThread
{
	template<class T>
	using CLock_queue=CBasic_ts_container<Lock_queue<T,std::mutex>>;
}

#endif