#include"../header/thread/CSpin_mutex.hpp"
using namespace std;

namespace nThread
{
	CSpin_mutex::CSpin_mutex() noexcept
		:flag_{ATOMIC_FLAG_INIT}{}

	void CSpin_mutex::lock() noexcept
	{
		while(flag_.test_and_set(memory_order_acquire))
			;
	}
}