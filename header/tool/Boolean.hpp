#ifndef BOOLEAN
#define BOOLEAN

namespace nTool
{
	class Boolean
	{
		bool bit_;
	public:
		constexpr Boolean() noexcept
			:bit_{false}{}
		constexpr Boolean(const bool bit) noexcept
			:bit_{bit}{}
		Boolean(const Boolean &)=default;
		constexpr operator bool() const noexcept
		{
			return bit_;
		}
		inline operator bool&() noexcept
		{
			return bit_;
		}
		Boolean& operator=(const Boolean &)=default;
	};
}

#endif