#ifndef CTHREAD_FORWARD_LIST
#define CTHREAD_FORWARD_LIST
#include<condition_variable>
#include<forward_list>
#include<memory>	//allocator
#include<mutex>
#include<utility>	//forward, move_if_noexcept

namespace nThread
{
	template<class T,class Alloc=std::allocator<T>>
	class CThread_forward_list	//a thread-safe std::forward_list
	{
	public:
		using allocator_type=Alloc;
		using value_type=T;
	private:
		std::condition_variable insert_;
		std::mutex insertMut_;
		std::forward_list<value_type,allocator_type> fwd_list_;
	public:
		CThread_forward_list()
			:CThread_forward_list{allocator_type()}{}
		explicit CThread_forward_list(const allocator_type &alloc)
			:fwd_list_{alloc}{}
		template<class ... Args>
		void emplace_front(Args &&...args)
		{
			bool notify;
			std::lock_guard<std::mutex> lock{insertMut_};
			notify=fwd_list_.empty();
			fwd_list_.emplace_front(std::forward<decltype(args)>(args)...);
			if(notify)
				insert_.notify_all();
		}
		inline bool empty() const noexcept
		{
			return fwd_list_.empty();
		}
		template<class UnaryPred>
		void remove_if(UnaryPred &&pred)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			fwd_list_.remove_if(std::forward<decltype(pred)>(pred));
		}
		value_type wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{insertMut_};
			insert_.wait(lock,[this]() noexcept{return !empty();});
			const auto temp{std::move_if_noexcept(fwd_list_.front())};
			fwd_list_.pop_front();
			lock.unlock();
			return temp;
		}
	};
}

#endif