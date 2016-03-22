#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr, make_shared
#include<utility>	//forward, move

namespace nThread
{
	//use std::shared_ptr to implement
	template<class T>
	class CAtomic_stack
	{
	public:
		using value_type=T;
	private:
		struct Node
		{
			value_type data;
			std::shared_ptr<Node> next;
			template<class ... Args>
			Node(const std::shared_ptr<Node> &next_,Args &&...args)
				:data{std::forward<decltype(args)>(args)...},next{next_}{}
		};
		std::shared_ptr<Node> begin_;
	public:
		template<class ... Args>
		void emplace(Args &&...args)
		{
			std::shared_ptr<Node> node{std::make_shared<Node>(begin_,std::forward<decltype(args)>(args)...)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node->next,node,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		inline bool empty() const noexcept
		{
			return !begin_.operator bool();
		}
		inline bool is_lock_free() const noexcept
		{
			return std::atomic_is_lock_free(&begin_);
		}
		value_type pop()
		{
			std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(node->data);
		}
	};
}

#endif