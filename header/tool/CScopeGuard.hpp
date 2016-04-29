#ifndef CSCOPEGUARD
#define CSCOPEGUARD
#include<functional>
#include<utility>	//forward, move

namespace nTool
{
	class CScopeGuard	//call function when exiting scope
	{
		const std::function<void()> func_;
	public:
		template<class Func,class ... Args>
		explicit CScopeGuard(Func &&func,Args &&...args)
			:func_{[&]() noexcept(noexcept(std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...))){
				std::forward<decltype(func)>(func)(std::forward<decltype(args)>(args)...);
			}}{}
		CScopeGuard(const CScopeGuard &)=delete;
		CScopeGuard& operator=(const CScopeGuard &)=delete;
		~CScopeGuard();
	};
}

#endif