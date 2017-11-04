#ifndef CSIGNAL_GUARD
#define CSIGNAL_GUARD
#include"../CUnique_obj.hpp"

namespace nTool
{
	class CSignal_guard
	{
		struct Impl;
		CUnique_obj<Impl> impl_;
	public:
		CSignal_guard();
		bool alive() const noexcept;
		void complete();
		~CSignal_guard();
	};
}

#endif