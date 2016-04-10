#ifndef NODE
#define NODE
#include<memory>	//allocator, shared_ptr
#include<utility>	//forward, move
#include<type_traits>

namespace nThread
{
	template<class T>
	struct Node
	{
		using allocator_type=std::allocator<T>;
		static allocator_type alloc;
		typename allocator_type::pointer data;
		std::shared_ptr<Node> next;
		Node()
			:data{alloc.allocate(1)}{}
		template<class ... Args>
		Node(Args &&...args)
			:Node{}
		{
			alloc.construct(data,std::forward<decltype(args)>(args)...);
		}
		template<class shared_ptrFwdRef,class ... Args,class=std::enable_if_t<std::is_same<std::remove_cv_t<std::remove_reference_t<shared_ptrFwdRef>>,std::shared_ptr<Node>>::value>>
		Node(shared_ptrFwdRef &&next_,Args &&...args)
			:data{alloc.allocate(1)},next{std::forward<decltype(next_)>(next_)}
		{
			alloc.construct(data,std::forward<decltype(args)>(args)...);
		}
		Node(const Node &)=delete;
		Node& operator=(const Node &)=delete;
		~Node()
		{
			alloc.destroy(data);
			alloc.deallocate(data,1);
			while(next.use_count()==1)
				next=std::move(next->next);
		}
	};

	template<class T>
	typename Node<T>::allocator_type Node<T>::Node::alloc;
}

#endif