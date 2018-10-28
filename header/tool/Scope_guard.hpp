#ifndef SCOPE_GUARD_HPP
#define SCOPE_GUARD_HPP
#include<functional>
#include<utility>	//forward

namespace nTool
{
	class Scope_guard	//call function when exiting scope
	{
		const std::function<void()> func_;
	public:
		template<class Func>
		explicit Scope_guard(Func &&func)
			:func_(std::forward<decltype(func)>(func)){}
		Scope_guard(const Scope_guard &)=delete;
		Scope_guard& operator=(const Scope_guard &)=delete;
		~Scope_guard();
	};
}

#endif