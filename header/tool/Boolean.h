#ifndef BOOLEAN
#define BOOLEAN

namespace nTool
{
	class Boolean
	{
		bool bit_;
	public:
		Boolean() noexcept;
		Boolean(bool) noexcept;
		Boolean(const Boolean &) noexcept=default;
		inline operator bool() const noexcept
		{
			return bit_;
		}
		inline operator bool&() noexcept
		{
			return bit_;
		}
		Boolean& operator=(bool) noexcept;
		Boolean& operator=(const Boolean &) noexcept=default;
	};
}

#endif