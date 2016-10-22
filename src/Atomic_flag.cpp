#include"../header/thread/Atomic_flag.hpp"

nThread::Atomic_flag::Atomic_flag() noexcept
	:flag{ATOMIC_FLAG_INIT}{}