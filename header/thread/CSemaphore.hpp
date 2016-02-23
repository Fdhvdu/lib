#ifndef CSEMAPHORE
#define CSEMAPHORE
#include<cstddef>
#include"../tool/CPimpl.hpp"

namespace nThread
{
	class CSemaphore	//semaphore
	{
	public:
		using size_type=std::size_t;
	private:
		class Impl;
		nTool::CPimpl<Impl> impl_;
	public:
		//CSemaphore::CSemaphore()
		//	:CSemaphore{0}{}
		CSemaphore();
		explicit CSemaphore(size_type);
		CSemaphore(const CSemaphore &)=delete;
		size_type count() const noexcept;
		void signal();
		void wait();
		CSemaphore& operator=(const CSemaphore &)=delete;
		~CSemaphore();
	};

	class CReaders_Writers_Problem	//no as powerful as std::shared_mutex
	{
		class Impl;
		nTool::CPimpl<Impl> impl_;
	public:
		CReaders_Writers_Problem();
		CReaders_Writers_Problem(const CReaders_Writers_Problem &)=delete;
		void readBegin();
		void readEnd();
		void writeBegin();
		void writeEnd();
		CReaders_Writers_Problem& operator=(const CReaders_Writers_Problem &)=delete;
		~CReaders_Writers_Problem();
	};
}

#endif