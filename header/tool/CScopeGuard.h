#ifndef CSCOPEGUARD
#define CSCOPEGUARD
#include<functional>

namespace nTool
{
	template<class Func_t>
	class CScopeGuard	//call function when exiting scope
	{
		std::function<Func_t> func_;
		bool exec_;
	public:
		template<class Func,class ... Args>
		explicit CScopeGuard(Func &&,Args &&...);
		CScopeGuard(const CScopeGuard &)=delete;
		CScopeGuard(CScopeGuard &&) noexcept;
		void clear() noexcept;	//do not call func_
		CScopeGuard& operator=(const CScopeGuard &)=delete;
		~CScopeGuard();
	};
}

#include"CScopeGuard.cpp"

#endif