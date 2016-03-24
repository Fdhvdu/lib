#include"../header/thread/CSemaphore.hpp"
#include<atomic>
#include<condition_variable>
#include<mutex>
using namespace std;

namespace nThread
{
	struct CSemaphore::Impl
	{
		atomic<CSemaphore::size_type> count_;
		condition_variable cv_;
		mutex mut_;
		Impl(CSemaphore::size_type);
		void signal();
		void wait();
	};

	CSemaphore::Impl::Impl(const CSemaphore::size_type count)
		:count_{count}{}

	void CSemaphore::Impl::signal()
	{
		if(!count_++)
		{
			lock_guard<mutex> lock{mut_};
			cv_.notify_all();
		}
	}

	void CSemaphore::Impl::wait()
	{
		unique_lock<mutex> lock{mut_};
		cv_.wait(lock,[this]() noexcept{return count_;});
		--count_;
	}

	CSemaphore::CSemaphore()
		:CSemaphore{0}{}

	CSemaphore::CSemaphore(const CSemaphore::size_type count)
		:impl_{count}{}

	CSemaphore::size_type CSemaphore::count() const noexcept
	{
		return impl_.get().count_;
	}

	void CSemaphore::reset()
	{
		impl_.get().count_=0;
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