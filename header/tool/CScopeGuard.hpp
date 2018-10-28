#ifndef CSCOPEGUARD
#define CSCOPEGUARD
#include<functional>
#include<utility>	//forward

namespace nTool
{
	class CScopeGuard	//call function when exiting scope
	{
		const std::function<void()> func_;
	public:
		template<class Func>
		explicit CScopeGuard(Func &&func)
			:func_(std::forward<decltype(func)>(func)){}
		CScopeGuard(const CScopeGuard &)=delete;
		CScopeGuard& operator=(const CScopeGuard &)=delete;
		~CScopeGuard();
	};
}

#endif