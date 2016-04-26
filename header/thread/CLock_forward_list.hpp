#ifndef CLOCK_FORWARD_LIST
#define CLOCK_FORWARD_LIST
#include<atomic>
#include<memory>	//shared_ptr
#include<mutex>
//#include<shared_mutex>
#include<utility>	//forward, move, swap
#include"Atomic_stack.hpp"
#include"Default_node.hpp"

namespace nThread
{
	template<class T>
	class CLock_forward_list
	{
	public:
		using value_type=T;
	private:
		using node_type=typename Atomic_stack<value_type>::node_type;
		using shared_ptr=typename Atomic_stack<value_type>::shared_ptr;
		Atomic_stack<value_type> stack_;
		std::mutex mut_;
		//std::shared_mutex mut_;
		void acquire_lock_and_emplace_front_(shared_ptr &&val)
		{
			std::lock_guard<std::mutex> lock{mut_};
			//std::shared_lock<std::shared_mutex> lock{mut_};
			stack_.emplace(std::move(val));
		}
	public:
		class CNode
		{
			friend CLock_forward_list<value_type>;
			shared_ptr p_;
		public:
			CNode()
				:p_{std::make_shared<node_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CLock_forward_list()=default;
		CLock_forward_list(const CLock_forward_list &)=delete;
		template<class ... Args>
		inline void emplace_front(Args &&...args)
		{
			acquire_lock_and_emplace_front_(std::make_shared<node_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace_front(CNode &&val,Args &&...args)
		{
			node_type::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			acquire_lock_and_emplace_front_(std::move(val.p_));
		}
		//do not call CLock_forward_list::emplace_front, emplace_front_not_ts, CAtomic_stack::pop_front, CLock_forward_list::remove or CLock_forward_list::remove_if at same time
		template<class ... Args>
		inline void emplace_front_not_ts(Args &&...args)
		{
			stack_.emplace_not_ts(std::make_shared<node_type>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return stack_.empty();
		}
		value_type pop_front()
		{
			std::lock_guard<std::mutex> lock{mut_};
			//std::shared_lock<std::shared_mutex> lock{mut_};
			return *stack_.pop()->data;
		}
		inline void remove(const value_type &remove_val)
		{
			remove_if([&](const auto &val) noexcept{return val==remove_val;});
		}
		//will block CLock_forward_list::emplace_front and CLock_forward_list::pop_front
		template<class UnaryPred>
		void remove_if(const UnaryPred pred)
		{
			std::lock_guard<std::mutex> lock{mut_};
			//std::lock_guard<std::shared_mutex> lock{mut_};
			for(shared_ptr bef{stack_.begin},iter{bef};iter;)
				if(pred(*iter->data))
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