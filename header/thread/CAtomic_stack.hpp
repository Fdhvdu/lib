#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr
#include<utility>	//forward, move
#include"Node.hpp"

namespace nThread
{
	template<class T>
	class CAtomic_stack
	{
		template<class T>
		friend void reverse_move(CAtomic_stack<T> &,CAtomic_stack<T> &) noexcept;
	public:
		using value_type=T;
	private:
		std::shared_ptr<Node<value_type>> begin_;
		void emplace_shared_ptr_(std::shared_ptr<Node<value_type>> &&val) noexcept
		{
			val->next=std::atomic_load_explicit(&begin_,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		std::shared_ptr<Node<value_type>> pop_() noexcept
		{
			std::shared_ptr<Node<value_type>> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return node;
		}
	public:
		class CNode
		{
			friend CAtomic_stack<T>;
			std::shared_ptr<Node<value_type>> p_;
		public:
			CNode()
				:p_{std::make_shared<Node<value_type>>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			emplace_shared_ptr_(std::make_shared<Node<value_type>>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
		{
			Node<value_type>::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			emplace_shared_ptr_(std::move(val.p_));
		}
		//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
		//2. do not call CAtomic_stack::emplace and CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			begin_=std::make_shared<Node<value_type>>(begin_,std::forward<decltype(args)>(args)...);
		}
		inline bool empty() const noexcept
		{
			return !begin_.operator bool();
		}
		inline bool is_lock_free() const noexcept
		{
			return std::atomic_is_lock_free(&begin_);
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop() noexcept
		{
			return std::move(*pop_()->data);
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};

	template<class T>
	void reverse_move(CAtomic_stack<T> &src,CAtomic_stack<T> &des) noexcept
	{
		while(!src.empty())
			des.emplace_shared_ptr_(src.pop_());
	}
}

#endif