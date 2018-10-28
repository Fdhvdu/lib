#ifndef CALLOC_OBJ
#define CALLOC_OBJ
#include<memory>
#include<type_traits>
#include<utility>

namespace nTool
{
	template<class T,class Alloc>
	class CAlloc_obj
	{
	public:
		using allocator_type=Alloc;
		using value_type=T;
		using reference=T&;
		using const_reference=const T&;
		using pointer=typename std::allocator_traits<Alloc>::pointer;
		using size_type=typename std::allocator_traits<Alloc>::size_type;
	private:
		template<class S,class Arg1,class ... Args>
		struct First_is_
			:std::integral_constant<bool,std::is_same_v<S,std::remove_cv_t<std::remove_reference_t<Arg1>>>>
		{};
		template<class S,class ... Args>
		struct Only_one_parameter_:std::false_type{};
		template<class S,class Arg1>
		struct Only_one_parameter_<S,Arg1>
			:std::integral_constant<bool,std::is_same_v<S,std::remove_cv_t<std::remove_reference_t<Arg1>>>>
		{};
		allocator_type alloc_;
		pointer data_;
		bool has_not_destroy_;
	public:
		CAlloc_obj()
			:CAlloc_obj(allocator_type{}){}
		explicit CAlloc_obj(const allocator_type &alloc)
			:alloc_(alloc),data_(std::allocator_traits<allocator_type>::allocate(alloc_,1)),has_not_destroy_(false){}
		CAlloc_obj(const CAlloc_obj &)=delete;
		CAlloc_obj(CAlloc_obj &&rhs) noexcept(std::is_nothrow_move_constructible_v<allocator_type>)
			:alloc_(std::move(rhs.alloc_)),data_(rhs.data_),has_not_destroy_(rhs.has_not_destroy_)
		{
			rhs.data_=nullptr;
		}
		template<class ... Args,class=std::enable_if_t<
			!(First_is_<std::allocator_arg_t,Args...>::value
				||Only_one_parameter_<allocator_type,Args...>::value
				||Only_one_parameter_<CAlloc_obj,Args...>::value)
		>>
		CAlloc_obj(Args &&...args)
			:CAlloc_obj(std::allocator_arg,allocator_type{},std::forward<decltype(args)>(args)...){}
		template<class ... Args>
		CAlloc_obj(std::allocator_arg_t,const allocator_type &alloc,Args &&...args)
			:CAlloc_obj(alloc)
		{
			construct(std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		void construct(Args &&...args)
		{
			std::allocator_traits<allocator_type>::construct(alloc_,data_,std::forward<decltype(args)>(args)...);
			has_not_destroy_=true;
		}
		inline void destroy()
		{
			std::allocator_traits<allocator_type>::destroy(alloc_,data_);
			has_not_destroy_=false;
		}
		inline reference get()
		{
			return *data_;
		}
		inline const_reference get() const
		{
			return *data_;
		}
		inline bool has_not_destroy() const noexcept
		{
			return has_not_destroy_;
		}
		CAlloc_obj& operator=(const CAlloc_obj &)=delete;
		CAlloc_obj& operator=(CAlloc_obj &&rhs) noexcept(std::is_nothrow_swappable_v<allocator_type>)
		{
			if(this!=&rhs)
			{
				std::swap(alloc_,rhs.alloc_);
				std::swap(data_,rhs.data_);
				std::swap(has_not_destroy_,rhs.has_not_destroy_);
			}
			return *this;
		}
		~CAlloc_obj()
		{
			if(data_)
			{
				if(has_not_destroy())
					destroy();
				std::allocator_traits<allocator_type>::deallocate(alloc_,data_,1);
			}
		}
	};
}

#endif