#ifndef CSPIN_MUTEX
#define CSPIN_MUTEX
#include<atomic>

namespace nThread
{
	class CSpin_mutex
	{
		std::atomic_flag flag_;
	public:
		CSpin_mutex() noexcept;
		CSpin_mutex(const CSpin_mutex &)=delete;
		void lock() noexcept;
		inline bool try_lock() noexcept
		{
			return !flag_.test_and_set(std::memory_order_acquire);
		}
		inline void unlock() noexcept
		{
			flag_.clear(std::memory_order_release);
		}
		CSpin_mutex& operator=(const CSpin_mutex &)=delete;
	};
}

#endif