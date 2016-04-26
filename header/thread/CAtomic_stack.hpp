#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<memory>	//shared_ptr
#include<utility>	//forward, move
#include"Atomic_stack.hpp"
#include"Default_node.hpp"

namespace nThread
{
	template<class T>
	class CAtomic_stack
	{
	public:
		using value_type=T;
	private:
		Atomic_stack<value_type> stack_;
	public:
		class CNode
		{
			friend CAtomic_stack<value_type>;
			typename Atomic_stack<value_type>::shared_ptr p_;
		public:
			CNode()
				:p_{std::make_shared<Atomic_stack<value_type>::node_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			stack_.emplace(std::make_shared<Atomic_stack<value_type>::node_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			Atomic_stack<value_type>::node_type::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			stack_.emplace(std::move(val.p_));
		}
		//do not call CAtomic_stack::emplace, emplace_not_ts or CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			stack_.emplace_not_ts(std::make_shared<Atomic_stack<value_type>::node_type>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return stack_.empty();
		}
		inline bool is_lock_free() const noexcept
		{
			return stack_.is_lock_free();
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop() noexcept
		{
			return std::move(*stack_.pop()->data);
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};
}

#endif