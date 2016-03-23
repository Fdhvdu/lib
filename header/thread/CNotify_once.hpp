#ifndef CNOTIFY_ONCE
#define CNOTIFY_ONCE
#include"../tool/CPimpl.hpp"

namespace nThread
{
	class CNotify_once
	{
		struct Impl;
		nTool::CPimpl<Impl> impl_;
	public:
		CNotify_once();
		CNotify_once(const CNotify_once &)=delete;
		//1. after returning from notify, CNotify_once::valid will return true
		//2. do not call it twice
		void notify();
		//1. return false for default constructor
		//2. return true after returning from CNotify_once::notify
		//3. non-block
		bool valid() const noexcept;
		//block until CNotify_once::valid return true
		void wait() const;
		CNotify_once& operator=(const CNotify_once &)=delete;
		~CNotify_once();
	};
}

#endif