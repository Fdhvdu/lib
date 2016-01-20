#include"../header/thread/CSmartThread.h"
using namespace std;

namespace nThread
{
	thread::id CSmartThread::get_id() const noexcept
	{
		return thr_.get_id();
	}

	CSmartThread::~CSmartThread()
	{
		if(thr_.joinable())
			thr_.join();
	}
}