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
		struct only_CAtomic_stack_can_construct_CNode_t{};
		//static constexpr only_CAtomic_stack_can_construct_CNode_t only_CAtomic_stack_can_construct_CNode{};
		static only_CAtomic_stack_can_construct_CNode_t only_CAtomic_stack_can_construct_CNode;
	public:
		using value_type=T;
		class CNode
		{
			friend CAtomic_stack<T>;
			std::shared_ptr<CNode> next_;
		public:
			value_type data;
			template<class ... Args>
			CNode(only_CAtomic_stack_can_construct_CNode_t,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
				:data{std::forward<decltype(args)>(args)...}{}
			template<class shared_ptrFwdRef,class ... Args,class=std::enable_if_t<std::is_same<std::remove_const_t<std::remove_reference_t<shared_ptrFwdRef>>,std::shared_ptr<CNode>>::value>>
			CNode(only_CAtomic_stack_can_construct_CNode_t,shared_ptrFwdRef &&next,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
				:data{std::forward<decltype(args)>(args)...},next_{std::forward<decltype(next)>(next)}{}
			CNode(const CNode &)=delete;
			CNode& operator=(const CNode &)=delete;
			~CNode()
			{
				while(next_.use_count()==1)
				{
					std::shared_ptr<CNode> p{std::move(next_->next_)};
					next_.reset();
					next_=std::move(p);
				}
			}
		};
	private:
		std::shared_ptr<CNode> begin_;
	public:
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			emplace_shared_ptr(std::make_shared<CNode>(only_CAtomic_stack_can_construct_CNode,std::forward<decltype(args)>(args)...));
		}
		//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
		//2. do not call CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			begin_=std::make_shared<CNode>(only_CAtomic_stack_can_construct_CNode,begin_,std::forward<decltype(args)>(args)...);
		}
		void emplace_shared_ptr(std::shared_ptr<CNode> &&val) noexcept
		{
			val->next_=std::atomic_load_explicit(&begin_,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&val->next_,val,std::memory_order_release,std::memory_order_relaxed))
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
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop() noexcept
		{
			return std::move(pop_shared_ptr()->data);
		}
		std::shared_ptr<CNode> pop_shared_ptr() noexcept
		{
			std::shared_ptr<CNode> CNode{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&CNode,CNode->next_,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return CNode;
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};

	template<class T>
	typename CAtomic_stack<T>::only_CAtomic_stack_can_construct_CNode_t CAtomic_stack<T>::only_CAtomic_stack_can_construct_CNode;
}

#endif