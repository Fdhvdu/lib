#include"../header/thread/CSemaphore.hpp"
#include<atomic>
#include<condition_variable>
#include<mutex>
using namespace std;

namespace nThread
{
	struct CSemaphore::Impl
	{
		atomic<CSemaphore::size_type> count;
		condition_variable cv;
		atomic_flag only_one_notify;
		mutex mut;
		Impl(CSemaphore::size_type);
		void signal();
		void wait();
	};

	CSemaphore::Impl::Impl(const CSemaphore::size_type count_)
		:count{count_},only_one_notify{ATOMIC_FLAG_INIT}{}

	void CSemaphore::Impl::signal()
	{
		if(!count++&&!only_one_notify.test_and_set(std::memory_order_acquire))
		{
			lock_guard<mutex> lock{mut};
			cv.notify_all();
			only_one_notify.clear(std::memory_order_release);
		}
	}

	void CSemaphore::Impl::wait()
	{
		unique_lock<mutex> lock{mut};
		cv.wait(lock,[this]() noexcept{return count.operator CSemaphore::size_type();});
		--count;
	}

	CSemaphore::CSemaphore()
		:CSemaphore{0}{}

	CSemaphore::CSemaphore(const CSemaphore::size_type count)
		:impl_{count}{}

	CSemaphore::size_type CSemaphore::count() const noexcept
	{
		return impl_.get().count;
	}

	void CSemaphore::reset()
	{
		impl_.get().count=0;
	}

	void CSemaphore::signal()
	{
		impl_.get().signal();
	}

	void CSemaphore::wait()
	{
		impl_.get().wait();
	}

	CSemaphore::~CSemaphore()=default;
}