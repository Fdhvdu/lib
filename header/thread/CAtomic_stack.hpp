#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr
#include<utility>	//forward, move
#include<type_traits>
#include<iostream>
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
			template<class ... Args>
			Node(Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
				:data{std::forward<decltype(args)>(args)...}{}
			template<class shared_ptrFwdRef,class ... Args,class=std::enable_if_t<std::is_same<std::remove_cv_t<std::remove_reference_t<shared_ptrFwdRef>>,std::shared_ptr<Node>>::value>>
			Node(shared_ptrFwdRef &&next_,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
				:data{std::forward<decltype(args)>(args)...},next{std::forward<decltype(next_)>(next_)}{}
			Node(const Node &)=delete;
			Node& operator=(const Node &)=delete;
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
		void emplace_shared_ptr_(std::shared_ptr<Node> &&val) noexcept
		{
			val->next=std::atomic_load_explicit(&begin_,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
				;
		}
	public:
		class CNode
		{
			friend void CAtomic_stack<T>::emplace_CNode(CNode &&) noexcept;
			std::shared_ptr<Node> p_;
		public:
			template<class ... Args>
			CNode(Args &&...args)
				:p_{std::make_shared<Node>(std::forward<decltype(args)>(args)...)}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			value_type& get_data()
			{
				return p_->data;
			}
			CNode& operator=(const CNode &)=delete;
		};
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			emplace_shared_ptr_(std::make_shared<Node>(std::forward<decltype(args)>(args)...));
		}
		//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
		//2. do not call CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			begin_=std::make_shared<Node>(begin_,std::forward<decltype(args)>(args)...);
		}
		inline void emplace_CNode(CNode &&val) noexcept
		{
			emplace_shared_ptr_(std::move(val.p_));
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
			std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(node->data);
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};
}

#endif