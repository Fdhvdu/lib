#ifndef ATOMIC_STACK
#define ATOMIC_STACK
#include<atomic>
#include<memory>	//allocator, shared_ptr
#include<utility>	//move
#include"Node.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	template<class T,class Alloc=std::allocator<T>>
	struct Atomic_stack
	{
		using allocator_type=Alloc;
		using element_type=Node<nTool::CAlloc_obj<T,Alloc>>;
		using size_type=typename nTool::CAlloc_obj<T,Alloc>::size_type;
		using value_type=T;
		std::shared_ptr<element_type> begin;
		Atomic_stack()=default;
		Atomic_stack(const Atomic_stack &)=delete;
		//have to guarantee the std::shared_ptr<element_type> is unique
		void emplace(std::shared_ptr<element_type> &&val) noexcept
		{
			while(!val.unique())	//prevent ABA problem
				;
			val->next=std::atomic_load_explicit(&begin,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		//do not call Atomic_stack::emplace, emplace_not_ts, Atomic_stack::pop or Atomic_stack::pop_not_ts at same time
		void emplace_not_ts(std::shared_ptr<element_type> &&val) noexcept
		{
			//yes, don't need to prevent ABA problem
			val->next=begin;
			begin=std::move(val);
		}
		inline bool empty() const noexcept
		{
			return !begin.operator bool();
		}
		inline bool is_lock_free() const noexcept
		{
			return std::atomic_is_lock_free(&begin);
		}
		std::shared_ptr<element_type> pop() noexcept
		{
			std::shared_ptr<element_type> node{std::atomic_load_explicit(&begin,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return node;
		}
		//do not call Atomic_stack::emplace, Atomic_stack::emplace_not_ts, Atomic_stack::pop or pop_not_ts at same time
		std::shared_ptr<element_type> pop_not_ts() noexcept
		{
			const std::shared_ptr<element_type> node{std::move(begin)};
			begin=std::move(node->next);
			return node;
		}
		Atomic_stack& operator=(const Atomic_stack &)=delete;
	};
}

#endif