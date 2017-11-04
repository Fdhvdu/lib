#include"../../header/tool/experimental/CSignal_guard.hpp"
#include<atomic>
#include<memory>
#include"../../header/thread/CSemaphore.hpp"
using namespace std;

namespace nTool
{
	struct CSignal_guard::Impl
	{
		atomic<bool> alive_;
		nThread::CSemaphore sema_;
		Impl();
	};

	CSignal_guard::Impl::Impl()
		:alive_{true}{}

	CSignal_guard::CSignal_guard()=default;

	bool CSignal_guard::alive() const noexcept
	{
		return impl_->alive_.load(memory_order_acquire);
	}

	void CSignal_guard::complete()
	{
		impl_->sema_.signal();
	}

	CSignal_guard::~CSignal_guard()
	{
		impl_->alive_.store(false,memory_order_release);
		impl_->sema_.wait();
	}
}