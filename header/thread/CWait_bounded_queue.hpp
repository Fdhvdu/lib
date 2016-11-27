#ifndef CWAIT_BOUNDED_QUEUE
#define CWAIT_BOUNDED_QUEUE
#include<utility>
#include"CLock_bounded_queue.hpp"
#include"CSemaphore.hpp"

namespace nThread
{
	template<class T>
	class CWait_bounded_queue
	{
	public:
		using size_type=typename CLock_bounded_queue<T>::size_type;
		using value_type=T;
	private:
		CLock_bounded_queue<value_type> queue_;
		CSemaphore_with_sub sema_;
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
		//do not call CWait_bounded_queue::emplace_and_notify, emplace_not_ts or CWait_bounded_queue::wait_and_pop at same time
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