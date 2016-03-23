#include"../header/thread/CNotify_once.hpp"
#include<future>	//future, promise
using namespace std;

namespace nThread
{
	struct CNotify_once::Impl
	{
		promise<void> prom;
		future<void> fut;
		Impl()
			:fut{prom.get_future()}{}
	};

	CNotify_once::CNotify_once()
		:impl_{}{}

	void CNotify_once::notify()
	{
		impl_.get().prom.set_value();
	}

	bool CNotify_once::valid() const noexcept
	{
		return impl_.get().fut.valid();
	}

	void CNotify_once::wait() const
	{
		impl_.get().fut.wait();
	}

	CNotify_once::~CNotify_once()=default;
}