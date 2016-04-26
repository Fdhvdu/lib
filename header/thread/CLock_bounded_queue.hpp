#ifndef CLOCK_BOUNDED_QUEUE
#define CLOCK_BOUNDED_QUEUE
#include<atomic>
#include<memory>	//allocator, pointer
#include<utility>	//forward, move
#include<type_traits>
#include"Atomic_stack.hpp"
#include"Default_node.hpp"
#include"Lock_queue.hpp"
#include"../tool/CScopeGuard.hpp"

namespace nThread
{
	template<class T>
	class CLock_bounded_queue
	{
		struct Node
		{
			using allocator_type=std::allocator<T>;
			static allocator_type alloc;
			typename allocator_type::pointer data;
			bool has_not_destroy;
			std::shared_ptr<Node> next;
			Node()
				:data{alloc.allocate(1)},has_not_destroy{false}{}
			Node(const Node &)=delete;
			Node& operator=(const Node &)=delete;
			~Node()
			{
				if(has_not_destroy)
					alloc.destroy(data);
				alloc.deallocate(data,1);
				while(next.use_count()==1)
					next=std::move(next->next);
			}
		};
	public:
		using size_type=typename Node::allocator_type::size_type;
		using value_type=T;
	private:
		using shared_ptr=typename Atomic_stack<value_type,Node>::shared_ptr;
		Atomic_stack<value_type,Node> stack_;
		Lock_queue<value_type,Node> queue_;
		const size_type size_;
		template<class ... Args>
		inline void construct_(shared_ptr &node,std::true_type,Args &&...args) noexcept
		{
			Node::alloc.construct(node->data,std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		void construct_(shared_ptr &node,std::false_type,Args &&...args)
		{
			try
			{
				Node::alloc.construct(node->data,std::forward<decltype(args)>(args)...);
			}catch(...)
			{
				node->has_not_destroy=false;
				stack_.emplace(std::move(node));
				throw ;
			}
		}
		template<class ... Args>
		void destroy_and_construct_(shared_ptr &node,Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
		{
			if(node->has_not_destroy)
				Node::alloc.destroy(node->data);
			construct_(node,std::is_nothrow_constructible<value_type,Args...>{},std::forward<decltype(args)>(args)...);
			node->has_not_destroy=false;
		}
	public:
		explicit CLock_bounded_queue(size_type size)
			:size_{size}
		{
			while(size--)
				stack_.emplace_not_ts(std::make_shared<typename Atomic_stack<value_type,Node>::node_type>());
		}
		CLock_bounded_queue(const CLock_bounded_queue &)=delete;
		template<class ... Args>
		void emplace(Args &&...args)
		{
			shared_ptr node{stack_.pop()};
			destroy_and_construct_(node,std::forward<decltype(args)>(args)...);
			queue_.emplace(std::move(node));
		}
		//do not call CLock_bounded_queue::emplace, emplace_not_ts or CLock_bounded_queue::pop at same time
		template<class ... Args>
		void emplace_not_ts(Args &&...args)
		{
			shared_ptr node{stack_.pop_not_ts()};
			destroy_and_construct_(node,std::forward<decltype(args)>(args)...);
			queue_.emplace_not_ts(std::move(node));
		}
		inline bool empty() const noexcept
		{
			return queue_.empty();
		}
		inline size_type size() const noexcept
		{
			return size_;
		}
		value_type pop()
		{
			shared_ptr node{queue_.pop()};
			const nTool::CScopeGuard sg{[&,this]() noexcept{stack_.emplace(std::move(node));}};
			return *node->data;
		}
		CLock_bounded_queue& operator=(const CLock_bounded_queue &)=delete;
	};

	template<class T>
	typename CLock_bounded_queue<T>::Node::allocator_type CLock_bounded_queue<T>::Node::alloc;
}

#endif