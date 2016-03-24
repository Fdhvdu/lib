#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr, make_shared
#include<utility>	//forward, move

namespace nThread
{
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
			template<class shared_ptrFwdRef,class ... Args>
			Node(shared_ptrFwdRef &&next_,Args &&...args)
				:data{std::forward<decltype(args)>(args)...},next{std::forward<decltype(next_)>(next_)}{}
			~Node()
			{
				while(next.use_count()==1)
				{
					std::shared_ptr<Node> p{std::move(next->next)};
					next.reset();
					next=std::move(p);
				}
			}
		};
		std::shared_ptr<Node> begin_;
	public:
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		void emplace(Args &&...args)
		{
			const std::shared_ptr<Node> node{std::make_shared<Node>(std::atomic_load_explicit(&begin_,std::memory_order_relaxed),std::forward<decltype(args)>(args)...)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node->next,node,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
		//2. do not call CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			begin_=std::make_shared<Node>(begin_,std::forward<decltype(args)>(args)...);
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
		value_type pop() noexcept
		{
			std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(node->data);
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};
}

#endif