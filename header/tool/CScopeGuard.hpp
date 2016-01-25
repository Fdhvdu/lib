#ifndef CSCOPEGUARD
#define CSCOPEGUARD
#include<functional>
#include<utility>	//forward, move

namespace nTool
{
	template<class Func_t>
	class CScopeGuard	//call function when exiting scope
	{
		std::function<Func_t> func_;
		bool exec_;
	public:
		template<class Func,class ... Args>
		explicit CScopeGuard(Func &&func,Args &&... args)
			:func_{std::bind(std::forward<Func>(func),std::forward<Args>(args)...)},exec_{true}{}
		CScopeGuard(const CScopeGuard &)=delete;
		CScopeGuard(CScopeGuard &&rVal) noexcept
			:func_{std::move(rVal.func_)},exec_{rVal.exec_}
		{
			rVal.exec_=false;
		}
		void clear() noexcept	//do not call func_
		{
			exec_=false;
		}
		CScopeGuard& operator=(const CScopeGuard &)=delete;
		~CScopeGuard()
		{
			if(exec_)
				func_();
		}
	};
}

#endif