#ifndef CTASK
#define CTASK
#include<functional>	//bind
#include<future>
#include<utility>	//forward

namespace nThread
{
	template<class Ret>
	Ret make_function();

	template<class Ret>
	class CTask	//combine std::packaged_task and std::future together
		//actually, the behavior of this class acts like std::function
		//but it provides wait
	{
		std::packaged_task<decltype(make_function<Ret>)> task_;
		std::future<Ret> fut_;
	public:
		CTask() noexcept=default;
		template<class Func,class ... Args>
		explicit CTask(Func &&func,Args &&...args)
			:task_{std::bind(std::forward<Func>(func),std::forward<Args>(args)...)},fut_{task_.get_future()}{}
		CTask(const CTask &)=delete;
		CTask(CTask &&) noexcept=default;
		inline Ret get()
		{
			return fut_.get();
		}
		inline bool valid() const noexcept	//return true after calling init; otherwise, return false
											//for default constructor, it returns false
		{
			return fut_.valid();
		}
		inline void wait() const
		{
			fut_.wait();
		}
		inline void operator()()
		{
			task_();
		}
		CTask& operator=(const CTask &)=delete;
		CTask& operator=(CTask &&) noexcept=default;
	};
}

#endif