#ifndef CTHREADRINGBUF
#define CTHREADRINGBUF
#include<condition_variable>
#include<memory>	//allocator
#include<mutex>
#include<queue>
#include<utility>	//forward, move, move_if_noexcept, pair
#include"CAtomic_stack.hpp"
#include"../algorithm/algorithm.hpp"	//for_each_val

namespace nThread
{
	//a fixed-sized and cannot overwrite when buffer is full
	template<class T>
	class CThreadRingBuf
	{
	public:
		using allocator_type=std::allocator<T>;
		using size_type=typename std::allocator<T>::size_type;
		using value_type=T;
	private:
		using pointer=typename std::allocator<T>::pointer;
		static allocator_type alloc_;
		const pointer begin_;
		const size_type size_;
		std::mutex mut_;
		std::queue<pointer> queue_;
		std::condition_variable read_cv_;
		CAtomic_stack<std::pair<bool,pointer>> stack_;
	public:
		explicit CThreadRingBuf(const size_type size)
			:begin_{alloc_.allocate(size)},size_{size}
		{
			nAlgorithm::for_each_val(begin_,begin_+size,[this](const auto p){
				stack_.emplace_not_ts(false,p);
			});
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		inline size_type available() const noexcept
		{
			return static_cast<size_type>(queue_.size());
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type read()
		{
			std::unique_lock<std::mutex> lock{mut_};
			read_cv_.wait(lock,[this]() noexcept{return available();});
			const pointer p{queue_.front()};
			//1. if move constructor is noexcept, it is exception safety
			//2. if move constructor is not noexcept and copy constructor exists, it is exception safety
			//3. if move constructor is not noexcept and copy constructor does not exist, it may not be exception safety
			const auto temp{std::move_if_noexcept(*p)};
			queue_.pop();
			lock.unlock();
			stack_.emplace(true,p);
			return temp;
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		//can only be used when your write will not overwrite the data
		template<class ... Args>
		void write(Args &&...args)
		{
			std::shared_ptr<typename CAtomic_stack<std::pair<bool,pointer>>::CNode> p{stack_.pop_shared_ptr()};
			if(p->data.first)
			{
				alloc_.destroy(p->data.second);
				p->data.first=false;
			}
			try
			{
				alloc_.construct(p->data.second,std::forward<decltype(args)>(args)...);
			}catch(...)
			{
				stack_.emplace_shared_ptr(std::move(p));
				throw ;
			}
			std::lock_guard<std::mutex> lock{mut_};
			queue_.emplace(p->data.second);
			if(queue_.size()==1)
				read_cv_.notify_all();
		}
		~CThreadRingBuf()
		{
			while(!stack_.empty())
			{
				const std::pair<bool,pointer> p{stack_.pop()};
				if(p.first)
					alloc_.destroy(p.second);
			}
			while(available())
			{
				alloc_.destroy(queue_.front());
				queue_.pop();
			}
			alloc_.deallocate(begin_,size());
		}
	};

	template<class T>
	typename CThreadRingBuf<T>::allocator_type CThreadRingBuf<T>::alloc_;
}

#endif