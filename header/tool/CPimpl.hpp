#ifndef CPIMPL
#define CPIMPL
#include<memory>	//make_unique, unique_ptr
#include<utility>	//forward, move
#include<type_traits>	//is_nothrow_assignable

namespace nTool
{
	template<class T>
	class CPimpl	//a class to help you use pimpl easily
	{
		std::unique_ptr<T> p_;
	public:
		CPimpl()
			:p_{std::make_unique<T>()}{}
		CPimpl(const CPimpl &val)
			:p_{std::make_unique<T>(val.get())}{}
		CPimpl(CPimpl &&rVal) noexcept
			:p_{std::move(rVal.p_)}{}
		//as smart pointer, use () instead of {}
		template<class ... Args>
		CPimpl(Args &&...args)
			:p_(std::make_unique<T>(std::forward<Args>(args)...)){}
		inline T& get() const noexcept
		{
			return *p_;
		}
		explicit operator bool() const noexcept
		{
			return p_.operator bool();
		}
		CPimpl& operator=(const CPimpl &val) noexcept(std::is_nothrow_assignable<T,T&>::value)
		{
			get()=val.get();
			return *this;
		}
		CPimpl& operator=(CPimpl &&rVal) noexcept
		{
			p_=std::move(rVal.p_);
			return *this;
		}
		~CPimpl(){}
	};
}

#endif