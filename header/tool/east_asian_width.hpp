#ifndef EAST_ASIAN_WIDTH
#define EAST_ASIAN_WIDTH

namespace nTool
{
	//according to Unicode 8.0.0
	//http://www.unicode.org/reports/tr11/
	//http://www.unicode.org/Public/8.0.0/ucd/EastAsianWidth.txt
	//if you are using UTF-8, you have to convert UTF-8 character to Unicode

	bool is_ambiguous(unsigned long unicode) noexcept;
	bool is_fullwidth(unsigned long unicode) noexcept;
	bool is_halfwidth(unsigned long unicode) noexcept;
	//do not contain halfwidth
	bool is_narrow(unsigned long unicode) noexcept;
	bool is_neutral(unsigned long unicode) noexcept;
	//do not contain fullwidth
	bool is_wide(unsigned long unicode) noexcept;
}

#endif