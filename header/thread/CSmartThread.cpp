#include"CSmartThread.h"
#include<utility>

namespace nThread
{
	template<class Func,class ... Args>
	CSmartThread::CSmartThread(Func &&func,Args &&...args)
		:thr_{std::forward<Func>(func),std::forward<Args>(args)...}{}
}