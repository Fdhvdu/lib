#ifndef DEFAULT_NODE
#define DEFAULT_NODE
#include<memory>	//allocator, shared_ptr
#include<utility>	//forward, move
#include<type_traits>

namespace nThread
{
	template<class T>
	struct Default_node
	{
		using allocator_type=std::allocator<T>;
		static allocator_type alloc;
		typename allocator_type::pointer data;
		std::shared_ptr<Default_node> next;
		Default_node()
			:data{alloc.allocate(1)}{}
		template<class ... Args>
		Default_node(Args &&...args)
			:Default_node{}
		{
			alloc.construct(data,std::forward<decltype(args)>(args)...);
		}
		Default_node(const Default_node &)=delete;
		Default_node& operator=(const Default_node &)=delete;
		~Default_node()
		{
			alloc.destroy(data);
			alloc.deallocate(data,1);
			while(next.use_count()==1)
				next=std::move(next->next);
		}
	};

	template<class T>
	typename Default_node<T>::allocator_type Default_node<T>::Default_node::alloc;
}

#endif