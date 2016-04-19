#ifndef ATOMIC_FLAG
#define ATOMIC_FLAG
#include<atomic>

namespace nThread
{
	struct Atomic_flag
	{
		std::atomic_flag flag;
		Atomic_flag() noexcept
			:flag{ATOMIC_FLAG_INIT}{}
	};
}

#endif