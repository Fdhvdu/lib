#ifndef CSMARTTHREAD
#define CSMARTTHREAD
#include<thread>
#include<utility>	//forward

namespace nThread
{
	class CSmartThread	//a thread which will automatically join itself in destructor
	{
		std::thread thr_;
	public:
		using id=std::thread::id;
		CSmartThread()=default;
		template<class Func,class ... Args>
		explicit CSmartThread(Func &&func,Args &&...args)
			:thr_{std::forward<Func>(func),std::forward<Args>(args)...}{}
		CSmartThread(const CSmartThread &)=delete;
		CSmartThread(CSmartThread &&)=default;
		id get_id() const noexcept;
		CSmartThread& operator=(const CSmartThread &)=delete;
		CSmartThread& operator=(CSmartThread &&)=default;
		~CSmartThread();
	};
}

#endif