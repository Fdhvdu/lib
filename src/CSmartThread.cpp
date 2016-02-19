#include"../header/thread/CSmartThread.hpp"
using namespace std;

namespace nThread
{
	CSmartThread::id CSmartThread::get_id() const noexcept
	{
		return thr_.get_id();
	}

	CSmartThread::~CSmartThread()
	{
		if(thr_.joinable())
			thr_.join();
	}
}