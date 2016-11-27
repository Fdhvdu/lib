#include"../header/thread/CSemaphore.hpp"
#include<atomic>
#include<condition_variable>
#include<mutex>
using namespace std;

namespace nThread
{
	namespace nImpl
	{
		struct CSemaphore::Impl
		{
			atomic<CSemaphore::size_type> count;
			condition_variable cv;
			atomic_flag only_one_notify;
			mutex mut;
			Impl(CSemaphore::size_type);
			void signal();
			bool sub_if_greater_than_0() noexcept;
			void wait();
			void wait_with_sub();
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

		bool CSemaphore::Impl::sub_if_greater_than_0() noexcept
		{
			auto val{count.load(std::memory_order_relaxed)};
			do
			{
				if(!val)
					return false;
			}while(!count.compare_exchange_weak(val,val-1,std::memory_order_acquire,std::memory_order_relaxed));
			return true;
		}

		void CSemaphore::Impl::wait()
		{
			unique_lock<mutex> lock{mut};
			cv.wait(lock,[this]() noexcept{return count.operator CSemaphore::size_type();});
			--count;
		}

		void CSemaphore::Impl::wait_with_sub()
		{
			unique_lock<mutex> lock{mut};
			cv.wait(lock,[this]() noexcept{return sub_if_greater_than_0();});
		}

		CSemaphore::CSemaphore(const CSemaphore::size_type count)
			:impl_{count}{}

		CSemaphore::size_type CSemaphore::count() const noexcept
		{
			return impl_->count;
		}

		void CSemaphore::reset() noexcept
		{
			impl_->count=0;
		}

		void CSemaphore::signal()
		{
			impl_->signal();
		}

		bool CSemaphore::sub_if_greater_than_0() noexcept
		{
			return impl_->sub_if_greater_than_0();
		}

		void CSemaphore::wait()
		{
			impl_->wait();
		}

		void CSemaphore::wait_with_sub()
		{
			impl_->wait_with_sub();
		}

		CSemaphore::~CSemaphore()=default;
	}	
}