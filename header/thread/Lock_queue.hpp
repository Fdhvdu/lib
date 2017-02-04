#ifndef LOCK_QUEUE
#define LOCK_QUEUE
#include<memory>	//allocator, shared_ptr
#include<type_traits>
#include<utility>	//move
#include"Node.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	struct Use_pop_if_exist;
	struct Do_not_use_pop_if_exist;

	template<class T,class PopIfExist,class Mutex,class Alloc=std::allocator<T>>
	struct Lock_queue
	{
		using allocator_type=Alloc;
		using element_type=Node<nTool::CAlloc_obj<T,Alloc>>;
		using mutex_type=Mutex;
		using size_type=typename nTool::CAlloc_obj<T,Alloc>::size_type;
		using value_type=T;
		static constexpr bool POP_IF_EXIST{std::is_same<PopIfExist,Use_pop_if_exist>::value};
		std::shared_ptr<element_type> begin;
		std::shared_ptr<element_type> end;
		mutex_type mut;
		Lock_queue()
			:end{std::make_shared<element_type>()}
		{
			begin=end;
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
			return !std::atomic_load_explicit(&begin,std::memory_order_acquire)->next;
		}
		std::shared_ptr<element_type> pop()
		{
			std::lock_guard<mutex_type> lock{mut};
			return pop_(std::integral_constant<bool,POP_IF_EXIST>{});
		}
		std::shared_ptr<element_type> pop_if_exist()
		{
			using is_enable=std::enable_if_t<POP_IF_EXIST>;
			if(empty())
				return std::shared_ptr<element_type>{};
			std::lock_guard<mutex_type> lock{mut};
			if(empty())
				return std::shared_ptr<element_type>{};
			return pop_(std::true_type{});
		}
		//do not call other member functions (including const member functions) at same time
		inline std::shared_ptr<element_type> pop_not_ts() noexcept
		{
			return pop_(std::false_type{});
		}
		Lock_queue& operator=(const Lock_queue &)=delete;
	private:
		using check_UsePopIfExist_type=std::enable_if_t<std::is_same<PopIfExist,Use_pop_if_exist>::value||std::is_same<PopIfExist,Do_not_use_pop_if_exist>::value>;
		void emplace_(std::shared_ptr<element_type> &&val) noexcept
		{
			end->next=val;
			std::swap(end->data,val->data);
			end=std::move(val);
		}
		std::shared_ptr<element_type> pop_(std::true_type) noexcept
		{
			const std::shared_ptr<element_type> node{begin};
			std::atomic_store_explicit(&begin,node->next,std::memory_order_release);
			return node;
		}
		std::shared_ptr<element_type> pop_(std::false_type) noexcept
		{
			const std::shared_ptr<element_type> node{std::move(begin)};
			begin=node->next;
			return node;
		}
	};
}

#endif