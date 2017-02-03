#ifndef CBASIC_TS_CONTAINER
#define CBASIC_TS_CONTAINER
#include<memory>
#include<type_traits>
#include<utility>

namespace nThread
{
	template<class U>
	class CBasic_ts_container
	{
	public:
		using underlying_type=U;
		using value_type=typename underlying_type::value_type;
	private:
		using element_type=typename underlying_type::element_type;
		underlying_type container_;
	public:
		class CNode
		{
			friend CBasic_ts_container<underlying_type>;
			using element_type=typename underlying_type::element_type;
			std::shared_ptr<element_type> p_;
		public:
			CNode()
				:p_{std::make_shared<element_type>()}{}
			CNode(const CNode &)=delete;
			CNode(CNode &&)=default;
			CNode& operator=(const CNode &)=delete;
		};
		CBasic_ts_container()=default;
		CBasic_ts_container(const CBasic_ts_container &)=delete;
		template<class ... Args>
		inline void emplace(Args &&...args)
		{
			container_.emplace(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		template<class ... Args>
		void emplace(CNode &&val,Args &&...args) noexcept(noexcept(std::declval<underlying_type>().emplace(std::shared_ptr<element_type>{}))&&noexcept(std::is_nothrow_constructible<value_type,Args...>::value))
		{
			val.p_->data.construct(std::forward<decltype(args)>(args)...);
			container_.emplace(std::move(val.p_));
		}
		//do not call other member functions (including const member functions) at same time
		template<class ... Args>
		inline void emplace_not_ts(Args &&...args)
		{
			container_.emplace_not_ts(std::make_shared<element_type>(std::forward<decltype(args)>(args)...));
		}
		inline bool empty() const noexcept
		{
			return container_.empty();
		}
		inline bool is_lock_free() const noexcept
		{
			return container_.is_lock_free();
		}
		//if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop() noexcept(noexcept(std::declval<underlying_type>().pop()))
		{
			return std::move(container_.pop()->data.get());
		}
		//return true if it has an element; otherwise, return false
		bool pop_if_exist(value_type &val) noexcept(noexcept(std::declval<underlying_type>().pop_if_exist())&&noexcept(std::is_nothrow_move_assignable<value_type>::value))
		{
			std::shared_ptr<element_type> temp{container_.pop_if_exist()};
			if(temp)
			{
				val=std::move(temp->data.get());
				return true;
			}
			return false;
		}
		//1. do not call CBasic_ts_container::emplace, CBasic_ts_container::emplace_not_ts, CBasic_ts_container::pop, CBasic_ts_container::pop_if_exist or pop_not_ts at same time
		//2. if constructor or assignment operator you use here is not noexcept, it may not be exception safety
		inline value_type pop_not_ts() noexcept
		{
			return std::move(container_.pop_not_ts()->data.get());
		}
		CBasic_ts_container& operator=(const CBasic_ts_container &)=delete;
	};
}

#endif