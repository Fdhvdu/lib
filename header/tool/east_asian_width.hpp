#ifndef EAST_ASIAN_WIDTH
#define EAST_ASIAN_WIDTH
#include<unordered_map>

namespace nTool
{
	//according to Unicode 8.0.0
	//http://www.unicode.org/reports/tr11/
	//http://www.unicode.org/Public/8.0.0/ucd/EastAsianWidth.txt
	
	//if you are using UTF-8, you have to convert UTF-8 character to Unicode (UTF-32) to use these functions
	//for example,
	//string utf8_string{"\u6C34"};	//contain only one character (using UTF-8 encoding)
	//wstring_convert<codecvt_utf8<char32_t>,char32_t> utf8_to_utf32;
	//u32string utf32_string{utf8_to_utf32.from_bytes(utf8_string)};
	//is_ambiguous(u32string[0]);	//because utf8_string contain only one character, check u32string[0] is enough
	//
	//and is_ambiguous return false
	//but is_wide return true

	//1. read east_asian_width_property
	//2.
	//0 is neutral, N
	//1 is ambiguous, A
	//2 is halfwidth, H
	//3 is narrow, Na
	//4 is fullwidth, F
	//5 is wide, W
	//3. the maximum range of east_asian_width_property is [0,0x10ffff]
	extern std::unordered_map<unsigned long,char> east_asian_width_property;

	template<class T>
	inline bool is_ambiguous(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==1;
	}

	template<class T>
	inline bool is_fullwidth(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==4;
	}

	template<class T>
	inline bool is_halfwidth(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==2;
	}

	//do not contain halfwidth
	template<class T>
	inline bool is_narrow(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==3;
	}

	template<class T>
	inline bool is_neutral(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==0;
	}
	
	//do not contain fullwidth
	template<class T>
	inline bool is_wide(const T unicode) noexcept
	{
		return east_asian_width_property[unicode]==5;
	}
}

#endif