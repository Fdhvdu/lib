#ifndef CSMARTTHREAD
#define CSMARTTHREAD
#include<thread>

namespace nThread
{
	class CSmartThread	//a thread which will automatically join itself in destructor
	{
		std::thread thr_;
	public:
		CSmartThread() noexcept=default;
		template<class Func,class ... Args>
		explicit CSmartThread(Func &&,Args &&...);
		CSmartThread(const CSmartThread &)=delete;
		CSmartThread(CSmartThread &&) noexcept=default;
		std::thread::id get_id() const noexcept;
		CSmartThread& operator=(const CSmartThread &)=delete;
		CSmartThread& operator=(CSmartThread &&) noexcept=default;
		~CSmartThread();
	};
}

#include"CSmartThread.cpp"

#endif