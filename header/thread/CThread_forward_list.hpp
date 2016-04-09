#ifndef CTHREAD_FORWARD_LIST
#define CTHREAD_FORWARD_LIST
#include<atomic>
#include<condition_variable>
#include<memory>	//allocator, shared_ptr
#include<mutex>
#include<utility>	//forward, move, swap

namespace nThread
{
	template<class T>
	class CThread_forward_list
	{
	public:
		using allocator_type=std::allocator<T>;
		using value_type=T;
	private:
		struct Node
		{
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
		std::shared_ptr<Node> begin_;
		std::condition_variable cv_;
		std::mutex wait_mut_;
		void acquire_lock_and_emplace_front_(std::shared_ptr<Node> &&val)
		{
			std::lock_guard<std::mutex> lock{wait_mut_};
			emplace_front_(std::move(val));
		}
		void emplace_front_(std::shared_ptr<Node> &&val) noexcept
		{
			val->next=std::atomic_load_explicit(&begin_,std::memory_order_relaxed);
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
				;
		}
		void notify_()
		{
			std::lock_guard<std::mutex> lock{wait_mut_};
			cv_.notify_one();
		}
		value_type pop_front_() noexcept
		{
			std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
			while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
				;
			return std::move(*node->data);
		}
	public:
		class CNode
		{
			friend CThread_forward_list<value_type>;
			std::shared_ptr<Node> p_;
		public:
			CNode()
				:p_{std::make_shared<Node>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CThread_forward_list()=default;
		CThread_forward_list(const CThread_forward_list &)=delete;
		template<class ... Args>
		void emplace_front(Args &&...args)
		{
			auto temp{std::make_shared<Node>(std::forward<decltype(args)>(args)...)};
			acquire_lock_and_emplace_front_(std::move(temp));
		}
		template<class ... Args>
		void emplace_front(CNode &&val,Args &&...args)
		{
			Node::alloc.construct(val.p_->data,std::forward<decltype(args)>(args)...);
			acquire_lock_and_emplace_front_(std::move(val.p_));
		}
		template<class ... Args>
		void emplace_front_and_notify(Args &&...args)
		{
			emplace_front(std::forward<decltype(args)>(args)...);
			notify_();
		}
		//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
		//2. do not call CAtomic_stack::pop_front, CAtomic_stack::remove, CAtomic_stack::remove_if or CAtomic_stack::wait_and_pop_front at same time
		template<class ... Args>
		inline void emplace_front_not_ts(Args &&...args)
		{
			begin_=std::make_shared<Node>(begin_,std::forward<decltype(args)>(args)...);
		}
		inline bool empty() const noexcept
		{
			return !begin_.operator bool();
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type pop_front()
		{
			std::lock_guard<std::mutex> lock{wait_mut_};
			return pop_front_();
		}
		inline void remove(const value_type &remove_val)
		{
			remove_if([&](const auto &val) noexcept{return val==remove_val;});
		}
		//will block CAtomic_stack::emplace_front, CAtomic_stack::pop_front and CAtomic_stack::wait_and_pop_front
		template<class UnaryPred>
		void remove_if(const UnaryPred pred)
		{
			std::lock_guard<std::mutex> lock{wait_mut_};
			for(std::shared_ptr<Node> bef{begin_},iter{bef};iter;)
				if(pred(*iter->data))
					if(begin_==iter)
					{
						iter=iter->next;
						begin_=bef=iter;
					}
					else
					{
						std::swap(bef->next,iter->next);
						iter=bef->next;
					}
				else
				{
					bef=iter;
					iter=iter->next;
				}
		}
		//1. if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		//2. will block wait_and_pop_front
		value_type wait_and_pop_front()
		{
			std::unique_lock<std::mutex> lock{wait_mut_};
			cv_.wait(lock,[this]() noexcept{return !empty();});
			//1. if move constructor is noexcept, it is exception safety
			//2. if move constructor is not noexcept and copy constructor exists, it is exception safety
			//3. if move constructor is not noexcept and copy constructor does not exist, it may not be exception safety
			const auto temp{pop_front_()};	//is pop_front_ instead of pop_front
			lock.unlock();
			return temp;
		}
		CThread_forward_list& operator=(const CThread_forward_list &)=delete;
	};

	template<class T>
	typename CThread_forward_list<T>::allocator_type CThread_forward_list<T>::Node::alloc;
}

#endif