#ifndef LOCK_QUEUE
#define LOCK_QUEUE
#include<memory>	//shared_ptr
#include<mutex>
#include<utility>	//move

namespace nThread
{
	template<class>struct Default_node;

	template<class T,class Node=Default_node<T>>
	struct Lock_queue
	{
		using node_type=Node;
		using shared_ptr=std::shared_ptr<Node>;
		using value_type=T;
		shared_ptr begin;
		shared_ptr end;
		std::mutex mut;
		Lock_queue()=default;
		Lock_queue(const Lock_queue &)=delete;
		void emplace(shared_ptr &&val)
		{
			val->next.reset();
			std::lock_guard<std::mutex> lock{mut};
			emplace_(std::move(val));
		}
		//do not call Lock_queue::emplace, emplace_not_ts or Lock_queue::pop at same time
		inline void emplace_not_ts(shared_ptr &&val) noexcept
		{
			emplace_(std::move(val));
		}
		inline bool empty() const noexcept
		{
			return !begin.operator bool();
		}
		shared_ptr pop()
		{
			shared_ptr node;
			{
				std::lock_guard<std::mutex> lock{mut};
				node=begin;
				begin=std::move(begin->next);
				if(empty())
					end.reset();
			}
			return node;
		}
		Lock_queue& operator=(const Lock_queue &)=delete;
	private:
		void emplace_(shared_ptr &&val) noexcept
		{
			if(empty())
			{
				begin=val;
				end=std::move(val);
			}
			else
			{
				end->next=std::move(val);
				end=end->next;
			}
		}
	};
}

#endif