#ifndef LOCK_QUEUE
#define LOCK_QUEUE
#include<memory>	//allocator, shared_ptr
#include<mutex>
#include<utility>	//move
#include"Node.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	template<class T,class Alloc=std::allocator<T>>
	struct Lock_queue
	{
		using allocator_type=Alloc;
		using element_type=Node<nTool::CAlloc_obj<T,Alloc>>;
		using size_type=typename nTool::CAlloc_obj<T,Alloc>::size_type;
		using value_type=T;
		std::shared_ptr<element_type> begin;
		std::shared_ptr<element_type> end;
		std::mutex mut;
		Lock_queue()=default;
		Lock_queue(const Lock_queue &)=delete;
		void emplace(std::shared_ptr<element_type> &&val)
		{
			val->next.reset();
			std::lock_guard<std::mutex> lock{mut};
			emplace_(std::move(val));
		}
		//do not call Lock_queue::emplace, emplace_not_ts, Lock_queue::pop or Lock_queue::pop_not_ts at same time
		inline void emplace_not_ts(std::shared_ptr<element_type> &&val) noexcept
		{
			emplace_(std::move(val));
		}
		inline bool empty() const noexcept
		{
			return !begin.operator bool();
		}
		std::shared_ptr<element_type> pop()
		{
			std::lock_guard<std::mutex> lock{mut};
			return pop_not_ts();
		}
		std::shared_ptr<element_type> pop_if_exist()
		{
			if(empty())
				return std::shared_ptr<element_type>{};
			std::lock_guard<std::mutex> lock{mut};
			if(empty())
				return std::shared_ptr<element_type>{};
			return pop_not_ts();
		}
		//do not call Lock_queue::emplace, Lock_queue::emplace_not_ts, Lock_queue::pop or pop_not_ts at same time
		std::shared_ptr<element_type> pop_not_ts() noexcept
		{
			const std::shared_ptr<element_type> node{std::move(begin)};
			begin=node->next;
			if(empty())
				end.reset();
			return node;
		}
		Lock_queue& operator=(const Lock_queue &)=delete;
	private:
		void emplace_(std::shared_ptr<element_type> &&val) noexcept
		{
			if(empty())
			{
				begin=val;
				end=std::move(val);
			}
			else
			{
				end->next=std::move(val);
				end=end->next;
			}
		}
	};
}

#endif