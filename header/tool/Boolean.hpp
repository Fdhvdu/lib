#ifndef BOOLEAN_HPP
#define BOOLEAN_HPP

namespace nTool
{
	class Boolean
	{
		bool bit_;
	public:
		constexpr Boolean() noexcept
			:bit_(false){}
		constexpr Boolean(bool bit) noexcept
			:bit_(bit){}
		constexpr Boolean(const Boolean &rhs)=default;
		constexpr operator bool() const noexcept
		{
			return bit_;
		}
		constexpr Boolean& operator=(bool b) noexcept
		{
			bit_=b;
			return *this;
		}
		constexpr Boolean& operator=(const Boolean &rhs)=default;
	};
}

#endif