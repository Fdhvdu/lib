#ifndef CTHREAD_FORWARD_LIST
#define CTHREAD_FORWARD_LIST
#include<condition_variable>
#include<forward_list>
#include<memory>	//allocator
#include<mutex>
#include<utility>	//forward, move_if_noexcept
//#include<atomic>
//#include<condition_variable>
//#include<memory>	//shared_ptr
//#include<mutex>
//#include<shared_mutex>
//#include<utility>	//forward, move, swap

namespace nThread
{
	template<class T,class Alloc=std::allocator<T>>
	class CThread_forward_list	//a thread-safe std::forward_list
	{
	public:
		using allocator_type=Alloc;
		using value_type=T;
	private:
		std::condition_variable insert_;
		std::mutex insertMut_;
		std::forward_list<value_type,allocator_type> fwd_list_;
	public:
		CThread_forward_list()
			:CThread_forward_list{allocator_type()}{}
		explicit CThread_forward_list(const allocator_type &alloc)
			:fwd_list_{alloc}{}
		template<class ... Args>
		void emplace_front(Args &&...args)
		{
			bool notify;
			std::lock_guard<std::mutex> lock{insertMut_};
			notify=fwd_list_.empty();
			fwd_list_.emplace_front(std::forward<decltype(args)>(args)...);
			if(notify)
				insert_.notify_all();
		}
		inline bool empty() const noexcept
		{
			return fwd_list_.empty();
		}
		template<class UnaryPred>
		void remove_if(UnaryPred &&pred)
		{
			std::lock_guard<std::mutex> lock{insertMut_};
			fwd_list_.remove_if(std::forward<decltype(pred)>(pred));
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		value_type wait_and_pop_front()
		{
			std::unique_lock<std::mutex> lock{insertMut_};
			insert_.wait(lock,[this]() noexcept{return !empty();});
			//1. if move constructor is noexcept, it is exception safety
			//2. if move constructor is not noexcept and copy constructor exists, it is exception safety
			//3. if move constructor is not noexcept and copy constructor does not exist, it may not be exception safety
			const auto temp{std::move_if_noexcept(fwd_list_.front())};
			fwd_list_.pop_front();
			lock.unlock();
			return temp;
		}
	};

	//template<class T>
	//class CThread_forward_list
	//{
	//public:
	//	using value_type=T;
	//private:
	//	struct Node
	//	{
	//		value_type data;
	//		std::shared_ptr<Node> next;
	//		template<class ... Args>
	//		Node(Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
	//			:data{std::forward<decltype(args)>(args)...}{}
	//		template<class shared_ptrFwdRef,class ... Args,class=std::enable_if_t<std::is_same<std::remove_cv_t<std::remove_reference_t<shared_ptrFwdRef>>,std::shared_ptr<Node>>::value>>
	//		Node(shared_ptrFwdRef &&next_,Args &&...args) noexcept(std::is_nothrow_constructible<T,Args...>::value)
	//			:data{std::forward<decltype(args)>(args)...},next{std::forward<decltype(next_)>(next_)}{}
	//		~Node()
	//		{
	//			while(next.use_count()==1)
	//			{
	//				std::shared_ptr<Node> p{std::move(next->next)};
	//				next.reset();
	//				next=std::move(p);
	//			}
	//		}
	//	};
	//	std::shared_ptr<Node> begin_;
	//	std::condition_variable cv_;
	//	std::shared_mutex remove_mut_;
	//	std::mutex wait_mut_;
	//	void emplace_front_(std::shared_ptr<Node> &&val) noexcept
	//	{
	//		while(!std::atomic_compare_exchange_weak_explicit(&begin_,&val->next,val,std::memory_order_release,std::memory_order_relaxed))
	//			;
	//		std::lock_guard<std::mutex> lock{wait_mut_};
	//		cv_.notify_one();
	//	}
	//public:
	//	class CNode
	//	{
	//		friend void CThread_forward_list<T>::emplace_CNode_front(CNode &&val) noexcept;
	//		std::shared_ptr<Node> p_;
	//	public:
	//		template<class Arg,class=std::enable_if_t<!std::is_base_of<std::remove_reference_t<Arg>,CNode>::value>>
	//		CNode(Arg &&arg)
	//			:p_{std::make_shared<Node>(std::forward<decltype(arg)>(arg))}{}
	//		template<class ... Args,class=std::enable_if_t<(sizeof...(Args)!=1)>>
	//		CNode(Args &&...args)
	//			:p_{std::make_shared<Node>(std::forward<decltype(args)>(args)...)}{}
	//		CNode(const CNode &)=delete;
	//		CNode(CNode &&)=default;
	//		value_type& get_data()
	//		{
	//			return p_->data;
	//		}
	//		CNode& operator=(const CNode &)=delete;
	//	};
	//	CThread_forward_list()=default;
	//	CThread_forward_list(const CThread_forward_list &)=delete;
	//	void emplace_CNode_front(CNode &&val) noexcept
	//	{
	//		std::shared_lock<std::shared_mutex> lock{remove_mut_};
	//		emplace_front_(std::move(val.p_));
	//	}
	//	template<class ... Args>
	//	void emplace_front(Args &&...args)
	//	{
	//		std::shared_lock<std::shared_mutex> lock{remove_mut_};
	//		emplace_front_(std::make_shared<Node>(std::forward<decltype(args)>(args)...));
	//	}
	//	//1. do not call emplace_not_ts with greater than or equal to 2 threads at same time
	//	//2. do not call CAtomic_stack::pop_front, CAtomic_stack::remove, CAtomic_stack::remove_if or CAtomic_stack::wait_and_pop_front at same time
	//	template<class ... Args>
	//	inline void emplace_front_not_ts(Args &&...args)
	//	{
	//		begin_=std::make_shared<Node>(begin_,std::forward<decltype(args)>(args)...);
	//	}
	//	inline bool empty() const noexcept
	//	{
	//		return !begin_.operator bool();
	//	}
	//	//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
	//	value_type pop_front()
	//	{
	//		std::shared_lock<std::shared_mutex> lock{remove_mut_};
	//		std::shared_ptr<Node> node{std::atomic_load_explicit(&begin_,std::memory_order_relaxed)};
	//		while(!std::atomic_compare_exchange_weak_explicit(&begin_,&node,node->next,std::memory_order_acquire,std::memory_order_relaxed))
	//			;
	//		return std::move(node->data);
	//	}
	//	inline void remove(const value_type &remove_val)
	//	{
	//		remove_if([&](const auto &val) noexcept{return val==remove_val;});
	//	}
	//	//will block CAtomic_stack::emplace_front, CAtomic_stack::pop_front and CAtomic_stack::wait_and_pop_front
	//	template<class UnaryPred>
	//	void remove_if(const UnaryPred pred)
	//	{
	//		std::lock<std::shared_mutex> lock{remove_mut_};
	//		for(std::shared_ptr<Node> bef{begin_},iter{bef};iter;)
	//			if(pred(iter->data))
	//				if(begin_==iter)
	//				{
	//					iter=iter->next;
	//					begin_=bef=iter;
	//				}
	//				else
	//				{
	//					std::swap(bef->next,iter->next);
	//					iter=bef->next;
	//				}
	//			else
	//			{
	//				bef=iter;
	//				iter=iter->next;
	//			}
	//	}
	//	//1. if constructor or assignment operator you use here is not noexcept, it may not be exception safety
	//	//2. will block wait_and_pop_front
	//	value_type wait_and_pop_front()
	//	{
	//		std::unique_lock<std::mutex> lock{wait_mut_};
	//		cv_.wait(lock,[this]() noexcept{return !empty();});
	//		//1. if move constructor is noexcept, it is exception safety
	//		//2. if move constructor is not noexcept and copy constructor exists, it is exception safety
	//		//3. if move constructor is not noexcept and copy constructor does not exist, it may not be exception safety
	//		const auto temp{pop_front()};
	//		lock.unlock();
	//		return temp;
	//	}
	//	CThread_forward_list& operator=(const CThread_forward_list &)=delete;
	};
}

#endif