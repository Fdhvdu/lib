#include"../header/tool/Boolean.h"

namespace nTool
{
	Boolean::Boolean() noexcept
		:bit_{false}{}

	Boolean::Boolean(const bool bit) noexcept
		:bit_{bit}{}

	Boolean& Boolean::operator=(const bool bit) noexcept
	{
		bit_=bit;
		return *this;
	}
}