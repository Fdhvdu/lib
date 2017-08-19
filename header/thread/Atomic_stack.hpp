#ifndef ATOMIC_STACK
#define ATOMIC_STACK
#include<atomic>
#include<memory>	//allocator, shared_ptr
#include<type_traits>
#include<utility>	//move
#include"Node.hpp"
#include"tag.hpp"
#include"../tool/CAlloc_obj.hpp"

namespace nThread
{
	template<class T,class PopIfExist,class Alloc=std::allocator<T>>
	struct Atomic_stack
	{
		using allocator_type=typename nTool::CAlloc_obj<T,Alloc>::allocator_type;
		using element_type=Node<nTool::CAlloc_obj<T,Alloc>>;
		using size_type=typename nTool::CAlloc_obj<T,Alloc>::size_type;
		using value_type=typename nTool::CAlloc_obj<T,Alloc>::value_type;
		static constexpr bool POP_IF_EXIST{std::is_same<PopIfExist,Use_pop_if_exist>::value};
		std::shared_ptr<element_type> begin;
		Atomic_stack()=default;
		Atomic_stack(const Atomic_stack &)=delete;
		//you have to guarantee the std::shared_ptr<element_type> is unique
		void emplace(std::shared_ptr<element_type> &&val) noexcept
		{
			using namespace std;
			while(1<val.use_count())	//prevent ABA problem
				;
			val->next=atomic_load_explicit(&begin,memory_order_relaxed);
			while(!atomic_compare_exchange_weak_explicit(&begin,&val->next,val,memory_order_release,memory_order_relaxed))
				;
			count_.add();
		}
		//do not call other member functions (including const member functions) at same time
		void emplace_not_ts(std::shared_ptr<element_type> &&val) noexcept
		{
			//yes, don't need to prevent ABA problem
			val->next=begin;
			begin=std::move(val);
			count_.add();
		}
		inline bool empty() const noexcept
		{
			return !std::atomic_load_explicit(&begin,std::memory_order_acquire);
		}
		inline bool is_lock_free() const noexcept
		{
			return std::atomic_is_lock_free(&begin);
		}
		std::shared_ptr<element_type> pop() noexcept
		{
			count_.sub();
			return pop_();
		}
		//return std::shared_ptr<element_type>{} if empty() return true; otherwise, call pop()
		std::shared_ptr<element_type> pop_if_exist() noexcept
		{
			using is_enable=std::enable_if_t<POP_IF_EXIST>;
			if(count_.sub_if_greater_than_0())
				return pop_();
			return std::shared_ptr<element_type>{};
		}
		//do not call Atomic_stack::emplace, Atomic_stack::emplace_not_ts, Atomic_stack::pop or pop_not_ts at same time
		std::shared_ptr<element_type> pop_not_ts() noexcept
		{
			count_.sub();
			const std::shared_ptr<element_type> node{std::move(begin)};
			begin=std::move(node->next);
			return node;
		}
		inline size_type size() const noexcept
		{
			using is_enable=std::enable_if_t<POP_IF_EXIST>;
			return count_.size();
		}
		Atomic_stack& operator=(const Atomic_stack &)=delete;
	private:
		using check_UsePopIfExist_type=std::enable_if_t<std::is_same<PopIfExist,Use_pop_if_exist>::value||std::is_same<PopIfExist,Do_not_use_pop_if_exist>::value>;
		class Count
		{
			std::atomic<size_type> count_;
		public:
			Count() noexcept
				:count_{0}{}
			Count(const Count &)=delete;
			inline void add() noexcept
			{
				count_.fetch_add(1,std::memory_order_release);
			}
			inline size_type size() const noexcept
			{
				return count_.load(std::memory_order_acquire);
			}
			inline void sub() noexcept
			{
				count_.fetch_sub(1,std::memory_order_release);
			}
			bool sub_if_greater_than_0() noexcept
			{
				auto val{count_.load(std::memory_order_relaxed)};
				do
				{
					if(!val)
						return false;
				}while(!count_.compare_exchange_weak(val,val-1,std::memory_order_acquire,std::memory_order_relaxed));
				return true;
			}
			Count& operator=(const Count &)=delete;
		};
		struct Empty
		{
			inline void add() const noexcept{}
			inline void sub() const noexcept{}
		};
		std::conditional_t<POP_IF_EXIST,Count,Empty> count_;
		std::shared_ptr<element_type> pop_() noexcept
		{
			using namespace std;
			shared_ptr<element_type> node{atomic_load_explicit(&begin,memory_order_relaxed)};
			while(!atomic_compare_exchange_weak_explicit(&begin,&node,atomic_load(&node->next),memory_order_acquire,memory_order_relaxed))
				;
			return node;
		}
	};
}

#endif