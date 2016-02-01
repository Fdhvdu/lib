#ifndef CTHREADRINGBUF
#define CTHREADRINGBUF
#include<atomic>
#include<condition_variable>
#include<memory>	//allocator
#include<mutex>
#include<type_traits>	//make_unsigned
#include<utility>	//forward, move
#include"../algorithm/algorithm.hpp"	//for_each_val

namespace nThread
{
	//a fixed-sized and cannot overwrite when buffer is full
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
		std::condition_variable cv_;
		const pointer end_;
		std::mutex read_mut_;
		size_type read_subscript_;
		std::atomic<size_type> write_subscript_;
		//can only be used when your write will not overwrite the data
		template<class TFwdRef>
		void write_(TFwdRef &&val)
		{
			const auto write{write_subscript_++};
			{
				std::lock_guard<std::mutex> lock{read_mut_};
				cv_.notify_all();
			}
			if(write<size())
				alloc_.construct(begin_+write,std::forward<TFwdRef>(val));
			else
				begin_[write%size()]=std::forward<TFwdRef>(val);
			complete_[write%size()]=true;
		}
	public:
		explicit CThreadRingBuf(const size_type size)
			:begin_{alloc_.allocate(size)},complete_{std::make_unique<std::atomic<bool>[]>(size)},end_{begin_+size},read_subscript_{0},write_subscript_{0}{}
		value_type read()
		{
			size_type read;
			{
				std::unique_lock<std::mutex> lock{read_mut_};
				cv_.wait(lock,[&]{
					if(read_subscript_<write_subscript_)
					{
						read=((read_subscript_++)%size());
						return true;
					}
					return false;
				});
			}
			while(!complete_[read])
				;
			complete_[read]=false;
			return std::move(begin_[read]);
		}
		inline size_type size() const noexcept
		{
			return end_-begin_;
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
			nAlgorithm::for_each_val(begin_,end_,[&](const auto p){alloc_.destroy(p);});
			alloc_.deallocate(begin_,size());
		}
	};

	template<class T>
	typename CThreadRingBuf<T>::allocator_type CThreadRingBuf<T>::alloc_;
}

#endif