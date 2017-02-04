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
		using reference=value_type&;
		using const_reference=const value_type&;
		using pointer=typename std::allocator_traits<Alloc>::pointer;
		using size_type=typename std::allocator_traits<Alloc>::size_type;
	private:
		static allocator_type alloc_;
		pointer data_;
		bool has_not_destroy_;
	public:
		CAlloc_obj()
			:data_{alloc_.allocate(1)},has_not_destroy_{false}{}
		CAlloc_obj(const CAlloc_obj &)=delete;
		CAlloc_obj(CAlloc_obj &&val) noexcept
			:data_{val.data_},has_not_destroy_{val.has_not_destroy_}
		{
			val.data_=nullptr;
		}
		template<class ... Args>
		CAlloc_obj(Args &&...args)
			:CAlloc_obj{}
		{
			construct(std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		void construct(Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			alloc_.construct(data_,std::forward<decltype(args)>(args)...);
			has_not_destroy_=true;
		}
		inline void destroy() noexcept
		{
			alloc_.destroy(data_);
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
		CAlloc_obj& operator=(CAlloc_obj &&val) noexcept
		{
			std::swap(data_,val.data_);
			std::swap(has_not_destroy_,val.has_not_destroy_);
			return *this;
		}
		~CAlloc_obj()
		{
			if(data_)
			{
				if(has_not_destroy())
					destroy();
				alloc_.deallocate(data_,1);
			}
		}
	};

	template<class T,class Alloc>
	typename CAlloc_obj<T,Alloc>::allocator_type CAlloc_obj<T,Alloc>::alloc_;
}

#endif