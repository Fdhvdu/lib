#ifndef CATOMICRINGBUF
#define CATOMICRINGBUF
#include<atomic>
#include<cstdint>	//uint_fast8_t
#include<memory>	//allocator, unique_ptr
#include<stdexcept>	//runtime_error
#include<type_traits>
#include<utility>	//forward
#include"../algorithm/algorithm.hpp"	//for_each_val
#include"../tool/CScopeGuard.hpp"

namespace nThread
{
	//a fixed-sized ring buffer
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
		//0 is destroy, 1 is destroyable, 2 is waiting to destroy
		const std::unique_ptr<std::atomic<std::uint_fast8_t>[]> destroy_status_;
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
				destroy_status_[subscript].store(0);
				throw ;
			}
		}
		void destroy_(const size_type subscript) noexcept
		{
			if(destroy_status_[subscript].load())
				alloc_.destroy(begin_+subscript);
		}
		size_type fetch_add_read_()
		{
			size_type bef{read_.load()};
			while(!read_.compare_exchange_weak(bef,(bef+1)%size()))
				;
			while(destroy_status_[bef].load()==2)
				;
			if(destroy_status_[bef]==0)
				throw std::runtime_error{"CAtomicRingBuf::write constructed unsuccessful"};
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
			:begin_{alloc_.allocate(size)},destroy_status_{std::make_unique<std::atomic<std::uint_fast8_t>[]>(size)},read_{0},size_{size},write_{0}{}
		inline size_type available() const noexcept
		{
			return write_.load()-read_.load();
		}
		inline bool empty() const noexcept
		{
			return read_==write_;
		}
		inline value_type read()
		{
			const size_type subscript{fetch_add_read_()};
			nTool::CScopeGuard sg{[=]{destroy_status_[subscript].store(2);}};
			return begin_[subscript];
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		template<class ... Args>
		void write(Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			const size_type subscript{fetch_add_write_()};
			destroy_(subscript);
			construct_(subscript,std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...);
			destroy_status_[subscript].store(1);
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