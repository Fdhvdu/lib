#include"../header/thread/CSmartThread.h"
using namespace std;

namespace nThread
{
	CSmartThread::~CSmartThread()
	{
		if(thr_.joinable())
			thr_.join();
	}
}