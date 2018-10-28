#include"../header/thread/CSpin_mutex.hpp"
using namespace std;

namespace nThread
{
	void CSpin_mutex::lock() noexcept
	{
		while(flag_.test_and_set(memory_order_acquire))
			;
	}
}