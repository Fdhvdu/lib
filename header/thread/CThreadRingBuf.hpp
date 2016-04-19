#ifndef CTHREADRINGBUF
#define CTHREADRINGBUF
#include<utility>	//forward
#include"CAtomicRingBuf.hpp"
#include"CSemaphore.hpp"

namespace nThread
{
	//a fixed-sized ring buffer
	template<class T>
	class CThreadRingBuf
	{
	public:
		using size_type=typename CAtomicRingBuf<T>::size_type;
		using value_type=T;
	private:
		CAtomicRingBuf<T> ring_buf_;
		CSemaphore sema_;
	public:
		explicit CThreadRingBuf(const size_type size)
			:ring_buf_{size},sema_{size}{}
		CThreadRingBuf(const CThreadRingBuf &)=delete;
		inline size_type available() const noexcept
		{
			return ring_buf_.available();
		}
		inline bool empty() const noexcept
		{
			return ring_buf_.empty();
		}
		inline size_type size() const noexcept
		{
			return ring_buf_.size();
		}
		value_type wait_and_read()
		{
			sema_.wait();
			return ring_buf_.read();
		}
		template<class ... Args>
		void write_and_notify(Args &&...args)
		{
			ring_buf_.write(std::forward<decltype(args)>(args)...);
			sema_.signal();
		}
		CThreadRingBuf& operator=(const CThreadRingBuf &)=delete;
	};
}

#endif