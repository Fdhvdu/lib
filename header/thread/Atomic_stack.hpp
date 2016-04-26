#ifndef ATOMIC_STACK
#define ATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr
#include<utility>	//move

namespace nThread
{
	template<class>struct Default_node;

	template<class T,class Node=Default_node<T>>
	struct Atomic_stack
	{
		using node_type=Node;
		using shared_ptr=std::shared_ptr<Node>;
		using value_type=T;
		shared_ptr begin;
		Atomic_stack()=default;
		Atomic_stack(const Atomic_stack &)=delete;
		//have to guarantee the shared_ptr is unique
		void emplace(shared_ptr &&val) noexcept
		{
			while(!val.unique())	//prevent ABA problem
				;
			val->next=std::atomic_load_explicit(&begin,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		//do not call Atomic_stack::emplace, emplace_not_ts, Atomic_stack::pop or Atomic_stack::pop_not_ts at same time
		void emplace_not_ts(shared_ptr &&val) noexcept
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
		shared_ptr pop() noexcept
		{
			shared_ptr node{std::atomic_load_explicit(&begin,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return node;
		}
		//do not call Atomic_stack::emplace, Atomic_stack::emplace_not_ts, Atomic_stack::pop or pop_not_ts at same time
		shared_ptr pop_not_ts() noexcept
		{
			const shared_ptr node{std::move(begin)};
			begin=std::move(node->next);
			return node;
		}
		Atomic_stack& operator=(const Atomic_stack &)=delete;
	};
}

#endif