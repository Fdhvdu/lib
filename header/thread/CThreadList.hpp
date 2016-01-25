#ifndef CTHREADLIST
#define CTHREADLIST
#include<condition_variable>
#include<cstddef>
#include<list>
#include<mutex>
#include<utility>	//forward, move

namespace nThread
{
	template<class T>
	class CThreadList	//a thread-safe std::list
	{
		std::condition_variable insert_;
		std::mutex insertMut_;
		std::list<T> list_;
	public:
		template<class ... Args>
		void emplace_back(Args &&...args)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			list_.emplace_back(std::forward<Args>(args)...);
			insert_.notify_all();
		}
		template<class UnaryPred>
		void remove_if(const UnaryPred pred)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			list_.remove_if(pred);
		}
		inline std::size_t size() const noexcept
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