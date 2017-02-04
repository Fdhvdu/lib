#ifndef LOCK_QUEUE
#define LOCK_QUEUE
#include<memory>	//allocator, shared_ptr
#include<utility>	//move
#include"Node.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	template<class T,class Mutex,class Alloc=std::allocator<T>>
	struct Lock_queue
	{
		using allocator_type=Alloc;
		using element_type=Node<nTool::CAlloc_obj<T,Alloc>>;
		using mutex_type=Mutex;
		using size_type=typename nTool::CAlloc_obj<T,Alloc>::size_type;
		using value_type=T;
		std::shared_ptr<element_type> begin;
		std::shared_ptr<element_type> end;
		mutex_type mut;
		Lock_queue()
			:begin{make_shared<element_type>()},end{make_shared<element_type>()}
		{
			begin->next=end;
		}
		Lock_queue(const Lock_queue &)=delete;
		void emplace(std::shared_ptr<element_type> &&val)
		{
			val->next.reset();
			std::lock_guard<mutex_type> lock{mut};
			emplace_(std::move(val));
		}
		//do not call other member functions (including const member functions) at same time
		inline void emplace_not_ts(std::shared_ptr<element_type> &&val) noexcept
		{
			emplace_(std::move(val));
		}
		inline bool empty() const noexcept
		{
			return !std::atomic_load_explicit(&std::atomic_load_explicit(&begin->next,std::memory_order_acquire)->next,std::memory_order_acquire);
		}
		std::shared_ptr<element_type> pop()
		{
			std::lock_guard<mutex_type> lock{mut};
			return pop_();
		}
		std::shared_ptr<element_type> pop_if_exist()
		{
			if(empty())
				return std::shared_ptr<element_type>{};
			std::lock_guard<mutex_type> lock{mut};
			if(empty())
				return std::shared_ptr<element_type>{};
			return pop_();
		}
		//do not call other member functions (including const member functions) at same time
		std::shared_ptr<element_type> pop_not_ts() noexcept
		{
			const std::shared_ptr<element_type> node{std::move(begin->next)};
			begin->next=node->next;
			return node;
		}
		Lock_queue& operator=(const Lock_queue &)=delete;
	private:
		void emplace_(std::shared_ptr<element_type> &&val) noexcept
		{
			end->next=val;
			std::swap(end->data,val->data);
			end=std::move(val);
		}
		std::shared_ptr<element_type> pop_() noexcept
		{
			using namespace std;
			const shared_ptr<element_type> node{atomic_load_explicit(&begin->next,memory_order_acquire)};
			atomic_store_explicit(&begin->next,atomic_load_explicit(&node->next,memory_order_acquire),memory_order_release);
			return node;
		}
	};
}

#endif