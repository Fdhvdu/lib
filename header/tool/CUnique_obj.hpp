#ifndef CUNIQUE_OBJ
#define CUNIQUE_OBJ
#include<memory>	//make_unique, unique_ptr
#include<utility>	//forward, move
#include<type_traits>	//is_nothrow_assignable

namespace nTool
{
	template<class T>
	class CUnique_obj	//a class to help you use pimpl and unique_ptr easily
	{
	public:
		using element_type=T;
		using pointer=element_type*;
	private:
		std::unique_ptr<element_type> p_;
	public:
		CUnique_obj()
			:p_{std::make_unique<element_type>()}{}
		CUnique_obj(const CUnique_obj &val)
			:p_{std::make_unique<element_type>(*val)}{}
		CUnique_obj(CUnique_obj &&rVal) noexcept
			:p_{std::move(rVal.p_)}{}
		//as smart pointer, use () instead of {}
		template<class ... Args>
		CUnique_obj(Args &&...args)
			:p_(std::make_unique<element_type>(std::forward<decltype(args)>(args)...)){}
		inline pointer get() const noexcept
		{
			return p_.get();
		}
		inline explicit operator bool() const noexcept
		{
			return p_.operator bool();
		}
		inline element_type& operator*() const
		{
			return *p_;
		}
		inline pointer operator->() const noexcept
		{
			return get();
		}
		CUnique_obj& operator=(const CUnique_obj &val) noexcept(std::is_nothrow_copy_assignable<element_type>::value)
		{
			*p=*val;
			return *this;
		}
		CUnique_obj& operator=(CUnique_obj &&rVal) noexcept
		{
			std::swap(p_,rVal.p_);
			return *this;
		}
		~CUnique_obj(){}
	};
}

#endif