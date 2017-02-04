#ifndef CLOCK_FORWARD_LIST
#define CLOCK_FORWARD_LIST
#include<memory>	//shared_ptr
#include<mutex>
#include<shared_mutex>
#include<utility>	//forward, move, swap
#include"Atomic_stack.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	template<class T>
	class CLock_forward_list
	{
	public:
		using value_type=T;
	private:
		using element_type=typename Atomic_stack<value_type,Do_not_use_pop_if_exist>::element_type;
		Atomic_stack<value_type,Do_not_use_pop_if_exist> stack_;
		std::shared_mutex mut_;
		void acquire_lock_and_emplace_front_(std::shared_ptr<element_type> &&val)
		{
			std::shared_lock<std::shared_mutex> lock{mut_};
			stack_.emplace(std::move(val));
		}
	public:
		class CNode
		{
			friend CLock_forward_list<value_type>;
			std::shared_ptr<element_type> p_;
		public:
			CNode()
				:p_{std::make_shared<element_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CLock_forward_list()=default;
		CLock_forward_list(const CLock_forward_list &)=delete;
		template<class ... Args>
		inline void emplace_front(Args &&...args)
		{
			acquire_lock_and_emplace_front_(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace_front(CNode &&val,Args &&...args)
		{
			val.p_->data.construct(std::forward<decltype(args)>(args)...);
			acquire_lock_and_emplace_front_(std::move(val.p_));
		}
		//do not call CLock_forward_list::emplace_front, emplace_front_not_ts, CLock_forward_list::pop_front, CLock_forward_list::remove or CLock_forward_list::remove_if at same time
		template<class ... Args>
		inline void emplace_front_not_ts(Args &&...args)
		{
			stack_.emplace_not_ts(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return stack_.empty();
		}
		value_type pop_front()
		{
			std::shared_lock<std::shared_mutex> lock{mut_};
			return std::move(stack_.pop()->data.get());
		}
		inline void remove(const value_type &remove_val)
		{
			remove_if([&](const auto &val) noexcept{return val==remove_val;});
		}
		//will block CLock_forward_list::emplace_front and CLock_forward_list::pop_front
		template<class UnaryPred>
		void remove_if(UnaryPred &&pred)
		{
			std::lock_guard<std::shared_mutex> lock{mut_};
			for(std::shared_ptr<element_type> bef{stack_.begin},iter{bef};iter;)
				if(pred(iter->data.get()))
					if(stack_.begin==iter)
					{
						iter=iter->next;
						stack_.begin=bef=iter;
					}
					else
					{
						std::swap(bef->next,iter->next);
						iter=bef->next;
					}
				else
				{
					bef=iter;
					iter=iter->next;
				}
		}
		CLock_forward_list& operator=(const CLock_forward_list &)=delete;
	};
}

#endif