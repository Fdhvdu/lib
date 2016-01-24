#ifndef CPIMPL
#define CPIMPL
#include<memory>	//std::unique_ptr

namespace nTool
{
	template<class T>
	class CPimpl	//a class to help you use pimpl easily
	{
		std::unique_ptr<T> p_;
	public:
		CPimpl();
		CPimpl(const CPimpl &);
		CPimpl(CPimpl &&) noexcept;
		template<class ... Args>
		CPimpl(Args &&...);
		inline T& get() const noexcept
		{
			return *p_.get();
		}
		CPimpl& operator=(const CPimpl &);
		CPimpl& operator=(CPimpl &&) noexcept;
		explicit operator bool() const noexcept
		{
			return p_.operator bool();
		}
		~CPimpl();
	};
}

#include"CPimpl.cpp"

#endif