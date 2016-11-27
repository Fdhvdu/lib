#ifndef CCHRONO_TIMER
#define CCHRONO_TIMER
#include<chrono>
#include<utility>	//forward

namespace nTool
{
	class CChrono_timer
	{
		std::chrono::steady_clock::time_point begin_;
		std::chrono::steady_clock::time_point end_;
	public:
		inline std::chrono::hours::rep duration_hours() const
		{
			return std::chrono::duration_cast<std::chrono::hours>(end_-begin_).count();
		}
		inline std::chrono::minutes::rep duration_minutes() const
		{
			return std::chrono::duration_cast<std::chrono::minutes>(end_-begin_).count();
		}
		inline std::chrono::seconds::rep duration_seconds() const
		{
			return std::chrono::duration_cast<std::chrono::seconds>(end_-begin_).count();
		}
		inline std::chrono::milliseconds::rep duration_milliseconds() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(end_-begin_).count();
		}
		inline std::chrono::microseconds::rep duration_microseconds() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(end_-begin_).count();
		}
		inline std::chrono::nanoseconds::rep duration_nanoseconds() const
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(end_-begin_).count();
		}
		inline void start() noexcept
		{
			begin_=std::chrono::steady_clock::now();
		}
		inline void stop() noexcept
		{
			end_=std::chrono::steady_clock::now();
		}
	};

	template<class Func,class ... Args>
	CChrono_timer calc_time(Func &&func,Args &&...args)
	{
		CChrono_timer timer;
		timer.start();
		func(std::forward<decltype(args)>(args)...);
		timer.stop();
		return timer;
	}
}

#endif
