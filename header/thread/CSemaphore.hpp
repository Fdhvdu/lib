#ifndef CSEMAPHORE
#define CSEMAPHORE
#include<atomic>
#include<condition_variable>
#include<cstddef>
#include<mutex>
#include<type_traits>
#include"tag.hpp"

namespace nThread
{
	template<class SubIfGreaterThan0,class Mutex>
	class CBasic_semaphore
	{
	public:
		using mutex_type=Mutex;
		using size_type=std::size_t;
		static constexpr bool SUB_IF_GREATER_THAN_0{std::is_same<SubIfGreaterThan0,Use_sub_if_greater_than_0>::value};
	private:
		using check_SubIfGreaterThan0_type=std::enable_if_t<std::is_same<SubIfGreaterThan0,Use_sub_if_greater_than_0>::value||std::is_same<SubIfGreaterThan0,Do_not_use_sub_if_greater_than_0>::value>;
		std::atomic<size_type> count_;
		std::conditional_t<std::is_same<mutex_type,std::mutex>::value,std::condition_variable,std::condition_variable_any> cv_;
		std::atomic_flag only_one_notify_=ATOMIC_FLAG_INIT;
		mutex_type mut_;
		void wait_(std::true_type)
		{
			std::unique_lock<mutex_type> lock(mut_);
			cv_.wait(lock,[this]() noexcept{return sub_if_greater_than_0();});
		}
		void wait_(std::false_type)
		{
			std::unique_lock<mutex_type> lock(mut_);
			cv_.wait(lock,[this]() noexcept{return count();});
			--count_;
		}
	public:
		CBasic_semaphore()
			:CBasic_semaphore(0){}
		explicit CBasic_semaphore(const size_type count)
			:count_(count){}
		CBasic_semaphore(const CBasic_semaphore &)=delete;
		inline size_type count() const noexcept
		{
			return count_;
		}
		//1. make count() become 0
		//2. will not signal any threads
		inline void reset() noexcept
		{
			count_.store(0);
		}
		//count() plus 1 and notify waiting threads
		void signal()
		{
			if(!count_++&&!only_one_notify_.test_and_set(std::memory_order_acquire))
			{
				std::lock_guard<mutex_type> lock(mut_);
				cv_.notify_all();
				only_one_notify_.clear(std::memory_order_release);
			}
		}
		//1. count() minus 1 if count() is greater than 0
		//2. do not wait
		//3. return true if count() is greater than 0 before minus; otherwise, return false
		bool sub_if_greater_than_0() noexcept
		{
			using is_enable=std::enable_if_t<SUB_IF_GREATER_THAN_0>;
			auto val(count_.load(std::memory_order_relaxed));
			do
			{
				if(!val)
					return false;
			}while(!count_.compare_exchange_weak(val,val-1,std::memory_order_acquire,std::memory_order_relaxed));
			return true;
		}
		//waiting until notifying if count() is 0; otherwise, count() minus 1
		inline void wait()
		{
			wait_(std::is_same<SubIfGreaterThan0,Use_sub_if_greater_than_0>{});
		}
		CBasic_semaphore& operator=(const CBasic_semaphore &)=delete;
	};

	using CSemaphore=CBasic_semaphore<Do_not_use_sub_if_greater_than_0,std::mutex>;
}

#endif