#ifndef CTHREADQUEUE
#define CTHREADQUEUE
#include<condition_variable>
#include<cstddef>
#include<queue>
#include<mutex>
#include<utility>	//forward, move

namespace nThread
{
	template<class T>
	class CThreadQueue	//a thread-safe std::queue
	{
	public:
		typedef T value_type;
		typedef typename std::queue<value_type>::size_type size_type;
	private:
		std::condition_variable push_;
		std::mutex pushMut_;
		std::queue<value_type> queue_;
	public:
		template<class ... Args>
		void emplace(Args &&...args)
		{
			std::lock_guard<std::mutex> lock{pushMut_};
			queue_.emplace(std::forward<Args>(args)...);
			push_.notify_all();
		}
		inline size_type size() const noexcept
		{
			return queue_.size();
		}
		value_type wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{pushMut_};
			push_.wait(lock,[this]{return size();});
			const auto temp{std::move(queue_.front())};
			queue_.pop();
			lock.unlock();
			return temp;
		}
	};
}

#endif