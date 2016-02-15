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
		explicit CScopeGuard(Func &&func,Args &&... args)
			:func_{std::bind(std::forward<Func>(func),std::forward<Args>(args)...)}{}
		CScopeGuard(const CScopeGuard &)=delete;
		CScopeGuard& operator=(const CScopeGuard &)=delete;
		~CScopeGuard()
		{
			func_();
		}
	};
}

#endif