#ifndef CSEMAPHORE
#define CSEMAPHORE
#include<cstddef>
#include"../tool/CPimpl.hpp"

namespace nThread
{
	class CSemaphore
	{
	public:
		using size_type=std::size_t;
	private:
		struct Impl;
		nTool::CPimpl<Impl> impl_;
	public:
		//CSemaphore::CSemaphore()
		//	:CSemaphore{0}{}
		CSemaphore();
		explicit CSemaphore(size_type);
		CSemaphore(const CSemaphore &)=delete;
		size_type count() const noexcept;
		//1. make CSemaphore::count become 0
		//2. will not signal any threads
		void reset();
		void signal();
		void wait();
		CSemaphore& operator=(const CSemaphore &)=delete;
		~CSemaphore();
	};
}

#endif