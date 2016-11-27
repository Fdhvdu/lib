#ifndef CSEMAPHORE
#define CSEMAPHORE
#include<cstddef>
#include<type_traits>
#include"../tool/CUnique_obj.hpp"

namespace nThread
{
	//this namespace is not for user
	namespace nImpl
	{
		class CSemaphore
		{
		public:
			using size_type=std::size_t;
		private:
			struct Impl;
			nTool::CUnique_obj<Impl> impl_;
		public:
			explicit CSemaphore(size_type);
			CSemaphore(const CSemaphore &)=delete;
			size_type count() const noexcept;
			void reset() noexcept;
			void signal();
			bool sub_if_greater_than_0() noexcept;
			void wait();
			void wait_with_sub();
			CSemaphore& operator=(const CSemaphore &)=delete;
			~CSemaphore();
		};
	}
	
	struct Use_sub_if_greater_than_0{};
	struct Do_not_use_sub_if_greater_than_0{};

	template<class UseSubIfGreaterThan0>
	class CBasic_semaphore
	{
	public:
		using size_type=nImpl::CSemaphore::size_type;
		enum Policy:bool
		{
			USE_SUB_IF_GREATER_THAN_0=std::is_same<UseSubIfGreaterThan0,Use_sub_if_greater_than_0>::value
		};
	private:
		using check_UseSubIfGreaterThan0_type=std::enable_if_t<std::is_same<UseSubIfGreaterThan0,Use_sub_if_greater_than_0>::value||std::is_same<UseSubIfGreaterThan0,Do_not_use_sub_if_greater_than_0>::value>;
		struct Wait
		{
			static void wait(nImpl::CSemaphore &sema)
			{
				sema.wait();
			}
		};
		struct Wait_with_sub
		{
			static void wait(nImpl::CSemaphore &sema)
			{
				sema.wait_with_sub();
			}
		};
		nImpl::CSemaphore sema_;
	public:
		CBasic_semaphore()
			:CBasic_semaphore{0}{}
		explicit CBasic_semaphore(const size_type count)
			:sema_{count}{}
		CBasic_semaphore(const CBasic_semaphore &)=delete;
		inline size_type count() const noexcept
		{
			return sema_.count();
		}
		//1. make count() become 0
		//2. will not signal any threads
		inline void reset() noexcept
		{
			sema_.reset();
		}
		//count() plus 1 and notify waiting threads
		inline void signal()
		{
			sema_.signal();
		}
		//1. count() minus 1 if count() is greater than 0
		//2. do not wait
		//3. return true if count() is greater than 0 before minus; otherwise, return false
		inline bool sub_if_greater_than_0() noexcept
		{
			using is_enable=std::enable_if_t<USE_SUB_IF_GREATER_THAN_0>;
			return sema_.sub_if_greater_than_0();
		}
		//waiting until notifying if count() is 0; otherwise, count() minus 1
		inline void wait()
		{
			std::conditional_t<USE_SUB_IF_GREATER_THAN_0,Wait_with_sub,Wait>::wait(sema_);	//for C++17, use if constexpr
		}
		CBasic_semaphore& operator=(const CBasic_semaphore &)=delete;
	};

	//notice, the definition of CBasic_semaphore will be changed in the future
	//but the name (CSemaphore and CSemaphore_with_sub) and tag (Do_not_use_sub_if_greater_than_0 and Use_sub_if_greater_than_0) will not be changed in the future
	using CSemaphore=CBasic_semaphore<Do_not_use_sub_if_greater_than_0>;
	using CSemaphore_with_sub=CBasic_semaphore<Use_sub_if_greater_than_0>;
}

#endif