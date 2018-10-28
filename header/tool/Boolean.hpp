#ifndef BOOLEAN
#define BOOLEAN

namespace nTool
{
	class Boolean
	{
		bool bit_;
	public:
		Boolean() noexcept
			:bit_(false){}
		Boolean(const bool bit) noexcept
			:bit_(bit){}
		Boolean(const Boolean &)=default;
		inline operator bool() const noexcept
		{
			return bit_;
		}
		Boolean& operator=(const bool b) noexcept
		{
			bit_=b;
			return *this;
		}
		Boolean& operator=(const Boolean &)=default;
	};
}

#endif