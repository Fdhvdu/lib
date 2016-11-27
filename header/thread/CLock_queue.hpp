#ifndef CLOCK_QUEUE
#define CLOCK_QUEUE
#include<memory>
#include<type_traits>
#include<utility>
#include"Lock_queue.hpp"

namespace nThread
{
	template<class T>
	class CLock_queue
	{
	public:
		using value_type=T;
	private:
		using underlying_type=Lock_queue<value_type>;
		using element_type=typename underlying_type::element_type;
		Lock_queue<value_type> queue_;
	public:
		class CNode
		{
			friend CLock_queue<value_type>;
			using element_type=typename Lock_queue<value_type>::element_type;
			std::shared_ptr<element_type> p_;
		public:
			CNode()
				:p_{std::make_shared<element_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CLock_queue()=default;
		CLock_queue(const CLock_queue &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			queue_.emplace(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args)
		{
			val.p_->data.construct(std::forward<decltype(args)>(args)...);
			queue_.emplace(std::move(val.p_));
		}
		//do not call CAtomic_stack::emplace, emplace_not_ts or CAtomic_stack::pop at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			queue_.emplace_not_ts(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop()
		{
			return std::move(queue_.pop()->data.get());
		}
		//return true if it has an element; otherwise, return false
		bool pop_if_exist(value_type &val)
		{
			std::shared_ptr<element_type> temp{queue_.pop_if_exist()};
			if(temp)
			{
				val=std::move(temp->data.get());
				return true;
			}
			return false;
		}
		CLock_queue& operator=(const CLock_queue &)=delete;
	};
}

#endif