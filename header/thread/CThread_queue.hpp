#ifndef CTHREAD_QUEUE
#define CTHREAD_QUEUE
#include<atomic>
#include<condition_variable>
#include<memory>	//shared_ptr
#include<mutex>
#include<utility>	//forward, move
#include"Node.hpp"

namespace nThread
{
	template<class T>
	class CThread_queue
	{
	public:
		using value_type=T;
	private:
		std::shared_ptr<Node<value_type>> begin_;
		std::condition_variable cv_;
		std::shared_ptr<Node<value_type>> end_;
		std::mutex mut_;
		void emplace_shared_ptr_(std::shared_ptr<Node<value_type>> &&val) noexcept
		{
			if(empty())
			{
				end_=val;
				std::atomic_store_explicit(&begin_,val,std::memory_order_release);
			}
			else
			{
				end_->next=std::move(val);
				end_=end_->next;
			}
		}
		void notify_()
		{
			std::lock_guard<std::mutex> lock{mut_};
			cv_.notify_one();
		}
	public:
		class CNode
		{
			friend CThread_queue<T>;
			std::shared_ptr<Node<value_type>> p_;
		public:
			CNode()
				:p_{std::make_shared<Node<value_type>>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CThread_queue()=default;
		CThread_queue(const CThread_queue &)=delete;
		template<class ... Args>
		void emplace(Args &&...args)
		{
			auto temp{std::make_shared<Node<value_type>>(std::forward<decltype(args)>(args)...)};
			std::lock_guard<std::mutex> lock{mut_};
			emplace_shared_ptr_(std::move(temp));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args)
		{
			Node<value_type>::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			std::lock_guard<std::mutex> lock{mut_};
			emplace_shared_ptr_(std::move(val.p_));
		}
		template<class ... Args>
		void emplace_and_notify(Args &&...args)
		{
			emplace(std::forward<decltype(args)>(args)...);
			notify_();
		}
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			emplace_shared_ptr_(std::make_shared<Node<value_type>>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return !begin_.operator bool();
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type pop() noexcept
		{
			std::shared_ptr<Node<value_type>> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(*node->data);
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type wait_and_pop()
		{
			std::unique_lock<std::mutex> lock{mut_};
			cv_.wait(lock,[this]() noexcept{return !empty();});
			return pop();
		}
		CThread_queue& operator=(const CThread_queue &)=delete;
	};
}

#endif