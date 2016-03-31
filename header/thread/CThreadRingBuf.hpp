#ifndef CTHREADRINGBUF
#define CTHREADRINGBUF
#include<condition_variable>
#include<memory>	//allocator
#include<mutex>
#include<queue>
#include<type_traits>
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
		using size_type=typename allocator_type::size_type;
		using value_type=T;
	private:
		using pointer=typename allocator_type::pointer;
		static allocator_type alloc_;
		const pointer begin_;
		const size_type size_;
		std::mutex mut_;
		std::queue<pointer> queue_;
		std::condition_variable read_cv_;
		CAtomic_stack<std::pair<bool,pointer>> stack_;
		pointer pop_and_destroy_() noexcept
		{
			std::pair<bool,pointer> p{stack_.pop()};
			if(p.first)
				alloc_.destroy(p.second);
			return p.second;
		}
		template<class ... Args>
		pointer write_(std::true_type,Args &&...args) noexcept
		{
			const pointer p{pop_and_destroy_()};
			alloc_.construct(p,std::forward<decltype(args)>(args)...);
			return p;
		}
		template<class ... Args>
		pointer write_(std::false_type,Args &&...args)
		{
			typename CAtomic_stack<std::pair<bool,pointer>>::CNode node;
			const pointer p{pop_and_destroy_()};
			try
			{
				alloc_.construct(p,std::forward<decltype(args)>(args)...);
			}catch(...)
			{
				stack_.emplace(std::move(node),false,p);
				throw ;
			}
			return p;
		}
		void acquire_lock_and_write_queue_(const pointer p)
		{
			std::lock_guard<std::mutex> lock{mut_};
			write_queue_(p);
		}
		void write_and_notify_queue_(const pointer p)
		{
			std::lock_guard<std::mutex> lock{mut_};
			write_queue_(p);
			if(queue_.size()==1)
				read_cv_.notify_all();
		}
		void write_queue_(const pointer p)
		{
			queue_.emplace(p);
		}
	public:
		explicit CThreadRingBuf(const size_type size)
			:begin_{alloc_.allocate(size)},size_{size}
		{
			nAlgorithm::for_each_val(begin_,begin_+size,[this](const auto p){
				stack_.emplace_not_ts(false,p);
			});
		}
		inline size_type available() const noexcept
		{
			return static_cast<size_type>(queue_.size());
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type read()
		{
			typename CAtomic_stack<std::pair<bool,pointer>>::CNode node;
			std::lock_guard<std::mutex> lock{mut_};
			const pointer p{queue_.front()};
			const auto temp{std::move_if_noexcept(*p)};
			queue_.pop();
			lock.unlock();
			stack_.emplace(std::move(node),true,p);
			return temp;
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type wait_and_read()
		{
			typename CAtomic_stack<std::pair<bool,pointer>>::CNode node;
			std::unique_lock<std::mutex> lock{mut_};
			read_cv_.wait(lock,[this]() noexcept{return available();});
			const pointer p{queue_.front()};
			const auto temp{std::move_if_noexcept(*p)};
			queue_.pop();
			lock.unlock();
			stack_.emplace(std::move(node),true,p);
			return temp;
		}
		//can only be used when your write will not overwrite the data
		template<class ... Args>
		inline void write(Args &&...args)
		{
			acquire_lock_and_write_queue_(write_(std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...));
		}
		//can only be used when your write will not overwrite the data
		template<class ... Args>
		inline void write_and_notify(Args &&...args)
		{
			write_and_notify_queue_(write_(std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		inline void write_not_ts(Args &&...args)
		{
			write_queue_(write_(std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...));
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