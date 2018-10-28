#ifndef NODE
#define NODE
#include<memory>	//shared_ptr
#include<type_traits>
#include<utility>	//forward, move

namespace nThread
{
	template<class T>
	struct Node
	{
		using value_type=T;
		value_type data;
		std::shared_ptr<Node> next;
		template<class ... Args>
		Node(Args &&...args) noexcept(std::is_nothrow_constructible<value_type,Args...>::value)
			:data(std::forward<decltype(args)>(args)...){}
		Node(const Node &)=delete;
		Node& operator=(const Node &)=delete;
		~Node()
		{
			while(next.use_count()==1)
			{
				auto temp(std::move(next->next));
				next=std::move(temp);
			}
		}
	};
}

#endif