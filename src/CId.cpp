#include"../header/tool/CId.hpp"

namespace nTool
{
	CId::CId() noexcept
		:CId{0}{}

	CId::CId(const size_type init) noexcept
		:id_{init}{}
}