#ifndef HOW_LONG_HPP
#define HOW_LONG_HPP
#include<chrono>
#include<functional>
#include<utility>	//forward

namespace nTool
{
	template<class Func,class ... Args>
	std::chrono::steady_clock::duration how_long(Func &&func,Args &&...args)
	{
		using namespace std;
		const auto timer_begin(chrono::steady_clock::now());
		invoke(forward<decltype(func)>(func),forward<decltype(args)>(args)...);
		return chrono::steady_clock::now()-timer_begin;
	}
}

#endif