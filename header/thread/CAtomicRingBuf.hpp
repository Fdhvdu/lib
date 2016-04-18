#ifndef CATOMICRINGBUF
#define CATOMICRINGBUF
#include<atomic>
#include<memory>	//allocator, unique_ptr
#include<type_traits>
#include<utility>	//forward
#include"../algorithm/algorithm.hpp"	//for_each_val

namespace nThread
{
	//1. fixed-sized
	//2. cannot overwrite when buffer is full
	template<class T>
	class CAtomicRingBuf
	{
	public:
		using allocator_type=std::allocator<T>;
		using size_type=typename allocator_type::size_type;
		using value_type=T;
	private:
		using pointer=typename allocator_type::pointer;
		static allocator_type alloc_;
		const pointer begin_;
		const std::unique_ptr<std::atomic<bool>[]> destroyable_;
		std::atomic<size_type> read_;
		const size_type size_;
		std::atomic<size_type> write_;
		template<class ... Args>
		inline void construct_(const size_type subscript,std::true_type,Args &&...args) noexcept
		{
			alloc_.construct(begin_+subscript,std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		void construct_(const size_type subscript,std::false_type,Args &&...args)
		{
			try
			{
				alloc_.construct(begin_+subscript,std::forward<decltype(args)>(args)...);
			}catch(...)
			{
				destroyable_[subscript].store(false);
				throw ;
			}
		}
		void destroy_(const size_type subscript) noexcept
		{
			if(destroyable_[subscript].load())
				alloc_.destroy(begin_+subscript);
		}
		size_type fetch_add_read_() noexcept
		{
			size_type bef{read_.load()};
			while(!(read_.compare_exchange_weak(bef,(bef+1)%size())&&(destroyable_[bef].load()||(bef=read_.load(),false))))
				;
			return bef;
		}
		size_type fetch_add_write_() noexcept
		{
			size_type bef{write_.load()};
			while(!write_.compare_exchange_weak(bef,(bef+1)%size()))
				;
			return bef;
		}
	public:
		explicit CAtomicRingBuf(const size_type size)
			:begin_{alloc_.allocate(size)},destroyable_{std::make_unique<std::atomic<bool>[]>(size)},read_{0},size_{size},write_{0}{}
		inline size_type available() const noexcept
		{
			return write_.load()-read_.load();
		}
		inline bool empty() const noexcept
		{
			return read_==write_;
		}
		inline value_type read() noexcept
		{
			return begin_[fetch_add_read_()];
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		template<class ... Args>
		void write(Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
		{
			const size_type subscript{fetch_add_write_()};
			destroy_(subscript);
			construct_(subscript,std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...);
			destroyable_[subscript].store(true);
		}
		~CAtomicRingBuf()
		{
			nAlgorithm::for_each_val<size_type>(0,size(),[this](const auto val){destroy_(val);});
			alloc_.deallocate(begin_,size());
		}
	};

	template<class T>
	typename CAtomicRingBuf<T>::allocator_type CAtomicRingBuf<T>::alloc_;
}

#endif