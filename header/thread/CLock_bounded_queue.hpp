#ifndef CLOCK_BOUNDED_QUEUE
#define CLOCK_BOUNDED_QUEUE
#include<memory>	//allocator, pointer
#include<mutex>
#include<type_traits>
#include<utility>	//forward, move
#include"Atomic_stack.hpp"
#include"Lock_queue.hpp"
#include"../tool/CAlloc_obj.hpp"
#include"../tool/CScopeGuard.hpp"

namespace nThread
{
	struct Use_pop_if_exist;
	struct Do_not_use_pop_if_exist;

	template<class T,class PopIfExist=Do_not_use_pop_if_exist>
	class CLock_bounded_queue
	{
	public:
		using size_type=typename Atomic_stack<T,PopIfExist>::size_type;
		using value_type=T;
	private:
		using element_type=typename Atomic_stack<value_type,PopIfExist>::element_type;
		const size_type bounded_size_;
		Atomic_stack<value_type,PopIfExist> stack_;
		Lock_queue<value_type,PopIfExist,std::mutex> queue_;
		template<class ... Args>
		inline void construct_(std::shared_ptr<element_type> &node,std::true_type,Args &&...args) noexcept
		{
			node->data.construct(std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		void construct_(std::shared_ptr<element_type> &node,std::false_type,Args &&...args)
		{
			try
			{
				node->data.construct(std::forward<decltype(args)>(args)...);
			}catch(...)
			{
				stack_.emplace(std::move(node));
				throw ;
			}
		}
		template<class ... Args>
		void destroy_and_construct_(std::shared_ptr<element_type> &node,Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			if(node->data.has_not_destroy())
				node->data.destroy();
			construct_(node,std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...);
		}
	public:
		explicit CLock_bounded_queue(size_type size)
			:bounded_size_{size}
		{
			while(size--)
				stack_.emplace_not_ts(std::make_shared<typename Atomic_stack<value_type,PopIfExist>::element_type>());
		}
		CLock_bounded_queue(const CLock_bounded_queue &)=delete;
		inline size_type bounded_size() const noexcept
		{
			return bounded_size_;
		}
		template<class ... Args>
		void emplace(Args &&...args)
		{
			std::shared_ptr<element_type> node{stack_.pop()};
			destroy_and_construct_(node,std::forward<decltype(args)>(args)...);
			queue_.emplace(std::move(node));
		}
		//do not call other member functions (including const member functions) at same time
		template<class ... Args>
		void emplace_not_ts(Args &&...args)
		{
			std::shared_ptr<element_type> node{stack_.pop_not_ts()};
			destroy_and_construct_(node,std::forward<decltype(args)>(args)...);
			queue_.emplace_not_ts(std::move(node));
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		value_type pop()
		{
			std::shared_ptr<element_type> node{queue_.pop()};
			const nTool::CScopeGuard sg{[&,this]() noexcept{stack_.emplace(std::move(node));}};
			return std::move(node->data.get());
		}
		bool pop_if_exist(value_type &val)
		{
			std::shared_ptr<element_type> node{queue_.pop_if_exist()};
			if(node)
			{
				val=std::move(node->data.get());
				stack_.emplace(std::move(node));
				return true;
			}
			return false;
		}
		CLock_bounded_queue& operator=(const CLock_bounded_queue &)=delete;
	};
}

#endif