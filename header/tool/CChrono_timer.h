#ifndef CCHRONO_TIMER
#define CCHRONO_TIMER
#include<chrono>

namespace nTool
{
	class CChrono_timer
	{
		std::chrono::high_resolution_clock::time_point begin_;
		std::chrono::high_resolution_clock::time_point end_;
	public:
		constexpr std::chrono::hours::rep duration_hours() const
		{
			return std::chrono::duration_cast<std::chrono::hours>(end_-begin_).count();
		}
		constexpr std::chrono::minutes::rep duration_minutes() const
		{
			return std::chrono::duration_cast<std::chrono::minutes>(end_-begin_).count();
		}
		constexpr std::chrono::seconds::rep duration_seconds() const
		{
			return std::chrono::duration_cast<std::chrono::seconds>(end_-begin_).count();
		}
		constexpr std::chrono::milliseconds::rep duration_milliseconds() const
		{
			return std::chrono::duration_cast<std::chrono::milliseconds>(end_-begin_).count();
		}
		constexpr std::chrono::microseconds::rep duration_microseconds() const
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(end_-begin_).count();
		}
		constexpr std::chrono::nanoseconds::rep duration_nanoseconds() const
		{
			return std::chrono::duration_cast<std::chrono::nanoseconds>(end_-begin_).count();
		}
		inline void start()
		{
			begin_=std::chrono::high_resolution_clock::now();
		}
		inline void stop()
		{
			end_=std::chrono::high_resolution_clock::now();
		}
	};
}

#endif