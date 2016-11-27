#ifndef CATOMIC_STACK
#define CATOMIC_STACK
#include<memory>	//shared_ptr
#include<utility>	//forward, move
#include<type_traits>
#include"Atomic_stack.hpp"

namespace nThread
{
	template<class T,class UsePopIfExist=Do_not_use_pop_if_exist>
	class CAtomic_stack
	{
	public:
		using value_type=T;
	private:
		using underlying_type=Atomic_stack<value_type,UsePopIfExist>;
		using element_type=typename underlying_type::element_type;
		underlying_type stack_;
	public:
		class CNode
		{
			friend CAtomic_stack<value_type>;
			using element_type=typename Atomic_stack<value_type,UsePopIfExist>::element_type;
			std::shared_ptr<element_type> p_;
		public:
			CNode()
				:p_{std::make_shared<element_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CAtomic_stack()=default;
		CAtomic_stack(const CAtomic_stack &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			stack_.emplace(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			val.p_->data.construct(std::forward<decltype(args)>(args)...);
			stack_.emplace(std::move(val.p_));
		}
		//do not call CAtomic_stack::emplace, emplace_not_ts or CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			stack_.emplace_not_ts(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
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
			return std::move(stack_.pop()->data.get());
		}
		//return true if it has an element; otherwise, return false
		bool pop_if_exist(value_type &val) noexcept(std::is_nothrow_move_assignable<value_type>::value)
		{
			using is_enable=std::enable_if_t<underlying_type::USE_POP_IF_EXIST>;
			std::shared_ptr<element_type> temp{stack_.pop_if_exist()};
			if(temp)
			{
				val=std::move(temp->data.get());
				return true;
			}
			return false;
		}
		CAtomic_stack& operator=(const CAtomic_stack &)=delete;
	};
}

#endif