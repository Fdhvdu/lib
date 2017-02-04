#ifndef CWAIT_BOUNDED_QUEUE
#define CWAIT_BOUNDED_QUEUE
#include<mutex>
#include<type_traits>
#include<utility>
#include"CLock_bounded_queue.hpp"
#include"CSemaphore.hpp"

namespace nThread
{
	struct Use_pop_if_exist;
	struct Do_not_use_pop_if_exist;

	template<class T,class PopIfExist=Use_pop_if_exist>
	class CWait_bounded_queue
	{
	public:
		using size_type=typename CLock_bounded_queue<T>::size_type;
		using value_type=T;
	private:
		CLock_bounded_queue<value_type,PopIfExist> queue_;
		std::conditional_t<std::is_same<PopIfExist,Use_pop_if_exist>::value,CBasic_semaphore<Use_sub_if_greater_than_0,std::mutex>,CSemaphore> sema_;
	public:
		explicit CWait_bounded_queue(const size_type size)
			:queue_{size}{}
		CWait_bounded_queue(const CWait_bounded_queue &)=delete;
		inline size_type bounded_size() const noexcept
		{
			return queue_.bounded_size();
		}
		template<class ... Args>
		void emplace_and_notify(Args &&...args)
		{
			queue_.emplace(std::forward<decltype(args)>(args)...);
			sema_.signal();
		}
		//do not call other member functions (including const member functions) at same time
		template<class ... Args>
		void emplace_not_ts(Args &&...args)
		{
			queue_.emplace_not_ts(std::forward<decltype(args)>(args)...);
			sema_.signal();
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		bool pop_if_exist(value_type &val)
		{
			if(sema_.sub_if_greater_than_0())
			{
				val=queue_.pop();
				return true;
			}
			return false;
		}
		value_type wait_and_pop()
		{
			sema_.wait();
			return queue_.pop();
		}
		CWait_bounded_queue& operator=(const CWait_bounded_queue &)=delete;
	};
}

#endif