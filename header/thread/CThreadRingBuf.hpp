#ifndef CTHREADRINGBUF
#define CTHREADRINGBUF
#include<atomic>
#include<memory>	//allocator, make_unique, unique_ptr
#include<utility>	//forward, move
#include"CSemaphore.hpp"
#include"../algorithm/algorithm.hpp"	//for_each_val

namespace nThread
{
	//a fixed-sized and cannot overwrite when buffer is full
	//T must meet the requirements of move constructor and move assignment operator
	template<class T>
	class CThreadRingBuf
	{
	public:
		typedef std::allocator<T> allocator_type;
		typedef typename std::allocator<T>::size_type size_type;
		typedef T value_type;
	private:
		typedef typename std::allocator<T>::pointer pointer;
		static allocator_type alloc_;
		const pointer begin_;
		std::unique_ptr<std::atomic<bool> []> complete_;
		std::atomic<size_type> read_subscript_;
		CSemaphore sema_;
		size_type size_;
		std::atomic<size_type> use_construct_;
		std::atomic<size_type> write_subscript_;
		//can only be used when your write will not overwrite the data
		template<class TFwdRef>
		void write_(TFwdRef &&val)
		{
			sema_.signal();
			const auto write{write_subscript_++};
			if(write<size()&&use_construct_++<size())
				alloc_.construct(begin_+write,std::forward<TFwdRef>(val));
			else
				begin_[write%size()]=std::forward<TFwdRef>(val);
			complete_[write%size()].store(true,std::memory_order_release);
		}
	public:
		explicit CThreadRingBuf(const size_type size)
			:begin_{alloc_.allocate(size)},complete_{std::make_unique<std::atomic<bool> []>(size)},sema_{0},size_{size},read_subscript_{0},use_construct_{0},write_subscript_{0}{}
		inline size_type available() const noexcept
		{
			return sema_.count();
		}
		value_type read()
		{
			sema_.wait();
			const auto read{(read_subscript_++)%size()};
			while(!complete_[read].load(std::memory_order_acquire))
				;
			complete_[read]=false;
			return std::move(begin_[read]);
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		inline void write(const T &val)
		{
			write_(val);
		}
		inline void write(T &&xval)
		{
			write_(std::move(xval));
		}
		~CThreadRingBuf()
		{
			nAlgorithm::for_each_val(begin_,begin_+size(),[&](const auto p){alloc_.destroy(p);});
			alloc_.deallocate(begin_,size());
		}
	};

	template<class T>
	typename CThreadRingBuf<T>::allocator_type CThreadRingBuf<T>::alloc_;
}

#endif