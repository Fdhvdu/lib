#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<atomic>
#include<memory>	//shared_ptr
#include<utility>	//forward, move
#include<type_traits>

namespace nThread
{
	template<class T>
	class CAtomic_stack
	{
	public:
		using allocator_type=std::allocator<T>;
		using value_type=T;
	private:
		struct Node
		{
			static allocator_type alloc;
			typename std::allocator<value_type>::pointer data;
			std::shared_ptr<Node> next;
			Node()
				:data{alloc.allocate(1)}{}
			template<class ... Args>
			Node(Args &&...args)
				:Node{}
			{
				alloc.construct(data,std::forward<decltype(args)>(args)...);
			}
			template<class shared_ptrFwdRef,class ... Args,class=std::enable_if_t<std::is_same<std::remove_cv_t<std::remove_reference_t<shared_ptrFwdRef>>,std::shared_ptr<Node>>::value>>
			Node(shared_ptrFwdRef &&next_,Args &&...args)
				:data{alloc.allocate(1)},next{std::forward<decltype(next_)>(next_)}
			{
				alloc.construct(data,std::forward<decltype(args)>(args)...);
			}
			Node(const Node &)=delete;
			Node& operator=(const Node &)=delete;
			~Node()
			{
				alloc.destroy(data);
				alloc.deallocate(data,1);
				while(next.use_count()==1)
				{
					alloc.destroy(next->data);
					alloc.deallocate(next->data,1);
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
			friend CAtomic_stack<T>;
			std::shared_ptr<Node> p_;
		public:
			CNode()
				:p_{std::make_shared<Node>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			emplace_shared_ptr_(std::make_shared<Node>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
		{
			Node::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			emplace_shared_ptr_(std::move(val.p_));
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
		inline value_type pop() noexcept
		{
			std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(*node->data);
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};

	template<class T>
	typename CAtomic_stack<T>::allocator_type CAtomic_stack<T>::Node::alloc;
}

#endif