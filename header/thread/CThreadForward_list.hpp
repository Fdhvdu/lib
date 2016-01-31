#ifndef CTHREADFORWARD_LIST
#define CTHREADFORWARD_LIST
#include<condition_variable>
#include<forward_list>
#include<memory>
#include<mutex>
#include<utility>	//forward, move

namespace nThread
{
	template<class T,class Alloc=std::allocator<T>>
	class CThreadForward_list	//a thread-safe std::forward_list
	{
	public:
		typedef Alloc allocator_type;
		typedef typename std::forward_list<T,Alloc>::size_type size_type;
		typedef T value_type;
	private:
		std::condition_variable insert_;
		std::mutex insertMut_;
		std::forward_list<value_type,allocator_type> fwd_list_;
	public:
		CThreadForward_list()
			:CThreadForward_list{allocator_type()}{}
		explicit CThreadForward_list(const allocator_type &alloc)
			:fwd_list_{alloc}{}
		template<class ... Args>
		void emplace_front(Args &&...args)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			fwd_list_.emplace_front(std::forward<Args>(args)...);
			insert_.notify_all();
		}
		template<class UnaryPred>
		void remove_if(const UnaryPred pred)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			fwd_list_.remove_if(pred);
		}
		inline size_type size() const noexcept
		{
			return list_.size();
		}
		T wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{insertMut_};
			insert_.wait(lock,[this]{return size();});
			const auto temp{std::move(list_.front())};
			list_.pop_front();
			lock.unlock();
			return temp;
		}
	};
}

#endif