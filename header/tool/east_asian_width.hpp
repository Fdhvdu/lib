#ifndef EAST_ASIAN_WIDTH
#define EAST_ASIAN_WIDTH

namespace nTool
{
	//according to Unicode 8.0.0
	//http://www.unicode.org/reports/tr11/
	//http://www.unicode.org/Public/8.0.0/ucd/EastAsianWidth.txt
	
	//if you are using UTF-8, you have to convert UTF-8 character to Unicode (UTF-32) to use these functions
	//for example,
	//string utf8_string{u8"\u6C34"};	//contain only one character (using UTF-8 encoding)
	//wstring_convert<codecvt_utf8<char32_t>,char32_t> utf8_to_utf32;
	//u32string utf32_string{utf8_to_utf32.from_bytes(utf8_string)};
	//is_ambiguous(u32string[0]);	//because utf8_string contain only one character, check u32string[0] is enough
	//
	//and is_ambiguous return false
	//but is_wide return true

	//although the parameter of is_ambiguous is unsigned long
	//but actually, only 21 bits are used
	//according to C++ standard, int (and unsigend int) is merely guarantee at least 16 bits
	//so I use long as parameter
	
	template<class T>
	bool is_ambiguous(const T unicode) noexcept
	{
		return
			(0xf0000<=unicode&&unicode<0xffffe)||
			(0x100000<=unicode&&unicode<0x10fffe)||
			(0xe000<=unicode&&unicode<0xf900)||
			(0xe0100<=unicode&&unicode<0xe01f0)||
			(0x2460<=unicode&&unicode<0x24ea)||
			(0x300<=unicode&&unicode<0x370)||
			(0x24eb<=unicode&&unicode<0x254c)||
			(0x410<=unicode&&unicode<0x450)||
			(0x1f130<=unicode&&unicode<0x1f16a)||
			(0x1f170<=unicode&&unicode<0x1f19b)||
			(0x2550<=unicode&&unicode<0x2574)||
			(0x1f110<=unicode&&unicode<0x1f12e)||
			(0x26e8<=unicode&&unicode<0x2700)||
			(0x26cf<=unicode&&unicode<0x26e2)||
			(0x391<=unicode&&unicode<0x3a2)||
			(0x3b1<=unicode&&unicode<0x3c2)||
			(0x2580<=unicode&&unicode<0x2590)||
			(0xfe00<=unicode&&unicode<0xfe10)||
			(0x2160<=unicode&&unicode<0x216c)||
			(0x1f100<=unicode&&unicode<0x1f10b)||
			(0x2170<=unicode&&unicode<0x217a)||
			(0x2190<=unicode&&unicode<0x219a)||
			(0x26c4<=unicode&&unicode<0x26ce)||
			(0x2776<=unicode&&unicode<0x2780)||
			(0x3248<=unicode&&unicode<0x3250)||
			(0x3a3<=unicode&&unicode<0x3aa)||
			(0x3c3<=unicode&&unicode<0x3ca)||
			(0x25a3<=unicode&&unicode<0x25aa)||
			(0x2227<=unicode&&unicode<0x222d)||
			(0xb0<=unicode&&unicode<0xb5)||
			(0xb6<=unicode&&unicode<0xbb)||
			(0x2b55<=unicode&&unicode<0x2b5a)||
			(0xbc<=unicode&&unicode<0xc0)||
			(0xde<=unicode&&unicode<0xe2)||
			(0xf7<=unicode&&unicode<0xfb)||
			(0x13f<=unicode&&unicode<0x143)||
			(0x148<=unicode&&unicode<0x14c)||
			(0x2d8<=unicode&&unicode<0x2dc)||
			(0x2013<=unicode&&unicode<0x2017)||
			(0x2024<=unicode&&unicode<0x2028)||
			(0x2081<=unicode&&unicode<0x2085)||
			(0x215b<=unicode&&unicode<0x215f)||
			(0x221d<=unicode&&unicode<0x2221)||
			(0x2234<=unicode&&unicode<0x2238)||
			(0x2264<=unicode&&unicode<0x2268)||
			(0x2592<=unicode&&unicode<0x2596)||
			(0x25ce<=unicode&&unicode<0x25d2)||
			(0x25e2<=unicode&&unicode<0x25e6)||
			(0x2667<=unicode&&unicode<0x266b)||
			(0xe8<=unicode&&unicode<0xeb)||
			(0x131<=unicode&&unicode<0x134)||
			(0x2c9<=unicode&&unicode<0x2cc)||
			(0x2020<=unicode&&unicode<0x2023)||
			(0x25c6<=unicode&&unicode<0x25c9)||
			(0x2663<=unicode&&unicode<0x2666)||
			(0xa7<=unicode&&unicode<0xa9)||
			(0xad<=unicode&&unicode<0xaf)||
			(0xd7<=unicode&&unicode<0xd9)||
			(0xec<=unicode&&unicode<0xee)||
			(0xf2<=unicode&&unicode<0xf4)||
			(0x126<=unicode&&unicode<0x128)||
			(0x152<=unicode&&unicode<0x154)||
			(0x166<=unicode&&unicode<0x168)||
			(0x2018<=unicode&&unicode<0x201a)||
			(0x201c<=unicode&&unicode<0x201e)||
			(0x2032<=unicode&&unicode<0x2034)||
			(0x2121<=unicode&&unicode<0x2123)||
			(0x2153<=unicode&&unicode<0x2155)||
			(0x21b8<=unicode&&unicode<0x21ba)||
			(0x2202<=unicode&&unicode<0x2204)||
			(0x2207<=unicode&&unicode<0x2209)||
			(0x223c<=unicode&&unicode<0x223e)||
			(0x2260<=unicode&&unicode<0x2262)||
			(0x226a<=unicode&&unicode<0x226c)||
			(0x226e<=unicode&&unicode<0x2270)||
			(0x2282<=unicode&&unicode<0x2284)||
			(0x2286<=unicode&&unicode<0x2288)||
			(0x25a0<=unicode&&unicode<0x25a2)||
			(0x25b2<=unicode&&unicode<0x25b4)||
			(0x25b6<=unicode&&unicode<0x25b8)||
			(0x25bc<=unicode&&unicode<0x25be)||
			(0x25c0<=unicode&&unicode<0x25c2)||
			(0x2605<=unicode&&unicode<0x2607)||
			(0x260e<=unicode&&unicode<0x2610)||
			(0x2614<=unicode&&unicode<0x2616)||
			(0x2660<=unicode&&unicode<0x2662)||
			(0x266c<=unicode&&unicode<0x266e)||
			(0x269e<=unicode&&unicode<0x26a0)||
			(0x26be<=unicode&&unicode<0x26c0)||
			0xa1==unicode||
			0xa4==unicode||
			0xaa==unicode||
			0xc6==unicode||
			0xd0==unicode||
			0xe6==unicode||
			0xf0==unicode||
			0xfc==unicode||
			0xfe==unicode||
			0x101==unicode||
			0x111==unicode||
			0x113==unicode||
			0x11b==unicode||
			0x12b==unicode||
			0x138==unicode||
			0x144==unicode||
			0x14d==unicode||
			0x16b==unicode||
			0x1ce==unicode||
			0x1d0==unicode||
			0x1d2==unicode||
			0x1d4==unicode||
			0x1d6==unicode||
			0x1d8==unicode||
			0x1da==unicode||
			0x1dc==unicode||
			0x251==unicode||
			0x261==unicode||
			0x2c4==unicode||
			0x2c7==unicode||
			0x2cd==unicode||
			0x2d0==unicode||
			0x2dd==unicode||
			0x2df==unicode||
			0x401==unicode||
			0x451==unicode||
			0x2010==unicode||
			0x2030==unicode||
			0x2035==unicode||
			0x203b==unicode||
			0x203e==unicode||
			0x2074==unicode||
			0x207f==unicode||
			0x20ac==unicode||
			0x2103==unicode||
			0x2105==unicode||
			0x2109==unicode||
			0x2113==unicode||
			0x2116==unicode||
			0x2126==unicode||
			0x212b==unicode||
			0x2189==unicode||
			0x21d2==unicode||
			0x21d4==unicode||
			0x21e7==unicode||
			0x2200==unicode||
			0x220b==unicode||
			0x220f==unicode||
			0x2211==unicode||
			0x2215==unicode||
			0x221a==unicode||
			0x2223==unicode||
			0x2225==unicode||
			0x222e==unicode||
			0x2248==unicode||
			0x224c==unicode||
			0x2252==unicode||
			0x2295==unicode||
			0x2299==unicode||
			0x22a5==unicode||
			0x22bf==unicode||
			0x2312==unicode||
			0x25cb==unicode||
			0x25ef==unicode||
			0x2609==unicode||
			0x261c==unicode||
			0x261e==unicode||
			0x2640==unicode||
			0x2642==unicode||
			0x266f==unicode||
			0x26e3==unicode||
			0x273d==unicode||
			0x2757==unicode||
			0xfffd==unicode;
	}

	template<class T>
	bool is_fullwidth(const T unicode) noexcept
	{
		return
			(0xff01<=unicode&&unicode<0xff61)||
			(0xffe0<=unicode&&unicode<0xffe7)||
			0x3000==unicode;
	}

	template<class T>
	bool is_halfwidth(const T unicode) noexcept
	{
		return
			(0xff61<=unicode&&unicode<0xffbf)||
			(0xffe8<=unicode&&unicode<0xffef)||
			(0xffc2<=unicode&&unicode<0xffc8)||
			(0xffca<=unicode&&unicode<0xffd0)||
			(0xffd2<=unicode&&unicode<0xffd8)||
			(0xffda<=unicode&&unicode<0xffdd)||
			0x20a9==unicode;
	}

	//do not contain halfwidth
	template<class T>
	bool is_narrow(const T unicode) noexcept
	{
		return
			(0x20<=unicode&&unicode<0x7f)||
			(0x27e6<=unicode&&unicode<0x27ee)||
			(0xa2<=unicode&&unicode<0xa4)||
			(0xa5<=unicode&&unicode<0xa7)||
			(0x2985<=unicode&&unicode<0x2987)||
			0xac==unicode||
			0xaf==unicode;
	}

	template<class T>
	bool is_neutral(const T unicode) noexcept
	{
		return
			(0x3fffe<=unicode&&unicode<0xe0100)||
			(0xe01f0<=unicode&&unicode<0xf0000)||
			(0xfffe<=unicode&&unicode<0x1b000)||
			(0x1b002<=unicode&&unicode<0x1f100)||
			(0x1160<=unicode&&unicode<0x2010)||
			(0x1f252<=unicode&&unicode<0x20000)||
			(0x452<=unicode&&unicode<0x1100)||
			(0xd7a4<=unicode&&unicode<0xe000)||
			(0xa4c7<=unicode&&unicode<0xa960)||
			(0x2b5a<=unicode&&unicode<0x2e80)||
			(0xfb00<=unicode&&unicode<0xfe00)||
			(0xa97d<=unicode&&unicode<0xac00)||
			(0x2987<=unicode&&unicode<0x2b55)||
			(0x27ee<=unicode&&unicode<0x2985)||
			(0x232b<=unicode&&unicode<0x2460)||
			(0xfe6c<=unicode&&unicode<0xff01)||
			(0x1dd<=unicode&&unicode<0x251)||
			(0x2780<=unicode&&unicode<0x27e6)||
			(0x1f19b<=unicode&&unicode<0x1f200)||
			(0x16c<=unicode&&unicode<0x1ce)||
			(0x262<=unicode&&unicode<0x2c4)||
			(0x20ad<=unicode&&unicode<0x2103)||
			(0x22c0<=unicode&&unicode<0x2312)||
			(0x4dc0<=unicode&&unicode<0x4e00)||
			(0x2700<=unicode&&unicode<0x273d)||
			(0x3ca<=unicode&&unicode<0x401)||
			(0x203f<=unicode&&unicode<0x2074)||
			(0x2670<=unicode&&unicode<0x269e)||
			(0x212c<=unicode&&unicode<0x2153)||
			(0x2085<=unicode&&unicode<0x20a9)||
			(0x7f<=unicode&&unicode<0xa1)||
			(0x370<=unicode&&unicode<0x391)||
			(0x261f<=unicode&&unicode<0x2640)||
			(0x0<=unicode&&unicode<0x20)||
			(0x2e0<=unicode&&unicode<0x300)||
			(0x219a<=unicode&&unicode<0x21b8)||
			(0x26a0<=unicode&&unicode<0x26be)||
			(0x2758<=unicode&&unicode<0x2776)||
			(0x2643<=unicode&&unicode<0x2660)||
			(0x2fd6<=unicode&&unicode<0x2ff0)||
			(0x22a6<=unicode&&unicode<0x22bf)||
			(0x273e<=unicode&&unicode<0x2757)||
			(0x21ba<=unicode&&unicode<0x21d2)||
			(0x21e8<=unicode&&unicode<0x2200)||
			(0x2313<=unicode&&unicode<0x2329)||
			(0xfe1a<=unicode&&unicode<0xfe30)||
			(0x25f0<=unicode&&unicode<0x2605)||
			(0x154<=unicode&&unicode<0x166)||
			(0x21d5<=unicode&&unicode<0x21e7)||
			(0x2270<=unicode&&unicode<0x2282)||
			(0x25d2<=unicode&&unicode<0x25e2)||
			(0x102<=unicode&&unicode<0x111)||
			(0x252<=unicode&&unicode<0x261)||
			(0x217a<=unicode&&unicode<0x2189)||
			(0x402<=unicode&&unicode<0x410)||
			(0xffef<=unicode&&unicode<0xfffd)||
			(0x2253<=unicode&&unicode<0x2260)||
			(0x2288<=unicode&&unicode<0x2295)||
			(0x1f203<=unicode&&unicode<0x1f210)||
			(0x2574<=unicode&&unicode<0x2580)||
			(0x2ef4<=unicode&&unicode<0x2f00)||
			(0x31e4<=unicode&&unicode<0x31f0)||
			(0x229a<=unicode&&unicode<0x22a5)||
			(0x11c<=unicode&&unicode<0x126)||
			(0x2075<=unicode&&unicode<0x207f)||
			(0x2117<=unicode&&unicode<0x2121)||
			(0x223e<=unicode&&unicode<0x2248)||
			(0x2596<=unicode&&unicode<0x25a0)||
			(0xc7<=unicode&&unicode<0xd0)||
			(0x210a<=unicode&&unicode<0x2113)||
			(0x25e6<=unicode&&unicode<0x25ef)||
			(0x2028<=unicode&&unicode<0x2030)||
			(0x25aa<=unicode&&unicode<0x25b2)||
			(0x114<=unicode&&unicode<0x11b)||
			(0x2d1<=unicode&&unicode<0x2d8)||
			(0x3aa<=unicode&&unicode<0x3b1)||
			(0x1f249<=unicode&&unicode<0x1f250)||
			(0xc0<=unicode&&unicode<0xc6)||
			(0xd1<=unicode&&unicode<0xd7)||
			(0x139<=unicode&&unicode<0x13f)||
			(0x2155<=unicode&&unicode<0x215b)||
			(0x218a<=unicode&&unicode<0x2190)||
			(0x2616<=unicode&&unicode<0x261c)||
			(0x1f16a<=unicode&&unicode<0x1f170)||
			(0xd9<=unicode&&unicode<0xde)||
			(0x12c<=unicode&&unicode<0x131)||
			(0x2036<=unicode&&unicode<0x203b)||
			(0x222f<=unicode&&unicode<0x2234)||
			(0x224d<=unicode&&unicode<0x2252)||
			(0x3100<=unicode&&unicode<0x3105)||
			(0x31bb<=unicode&&unicode<0x31c0)||
			(0x1f10b<=unicode&&unicode<0x1f110)||
			(0x1f23b<=unicode&&unicode<0x1f240)||
			(0xe2<=unicode&&unicode<0xe6)||
			(0x134<=unicode&&unicode<0x138)||
			(0x14e<=unicode&&unicode<0x152)||
			(0x2127<=unicode&&unicode<0x212b)||
			(0x216c<=unicode&&unicode<0x2170)||
			(0x2216<=unicode&&unicode<0x221a)||
			(0x2238<=unicode&&unicode<0x223c)||
			(0x254c<=unicode&&unicode<0x2550)||
			(0x25b8<=unicode&&unicode<0x25bc)||
			(0x25c2<=unicode&&unicode<0x25c6)||
			(0x260a<=unicode&&unicode<0x260e)||
			(0x2610<=unicode&&unicode<0x2614)||
			(0x26c0<=unicode&&unicode<0x26c4)||
			(0x26e4<=unicode&&unicode<0x26e8)||
			(0x2ffc<=unicode&&unicode<0x3000)||
			(0xf4<=unicode&&unicode<0xf7)||
			(0x128<=unicode&&unicode<0x12b)||
			(0x145<=unicode&&unicode<0x148)||
			(0x168<=unicode&&unicode<0x16b)||
			(0x2106<=unicode&&unicode<0x2109)||
			(0x2123<=unicode&&unicode<0x2126)||
			(0x2204<=unicode&&unicode<0x2207)||
			(0x220c<=unicode&&unicode<0x220f)||
			(0x2212<=unicode&&unicode<0x2215)||
			(0x2249<=unicode&&unicode<0x224c)||
			(0x2296<=unicode&&unicode<0x2299)||
			(0x312e<=unicode&&unicode<0x3131)||
			(0xa48d<=unicode&&unicode<0xa490)||
			(0xffbf<=unicode&&unicode<0xffc2)||
			(0xffdd<=unicode&&unicode<0xffe0)||
			(0xee<=unicode&&unicode<0xf0)||
			(0xff<=unicode&&unicode<0x101)||
			(0x2c5<=unicode&&unicode<0x2c7)||
			(0x2ce<=unicode&&unicode<0x2d0)||
			(0x2011<=unicode&&unicode<0x2013)||
			(0x201a<=unicode&&unicode<0x201c)||
			(0x201e<=unicode&&unicode<0x2020)||
			(0x203c<=unicode&&unicode<0x203e)||
			(0x20aa<=unicode&&unicode<0x20ac)||
			(0x2114<=unicode&&unicode<0x2116)||
			(0x2209<=unicode&&unicode<0x220b)||
			(0x221b<=unicode&&unicode<0x221d)||
			(0x2221<=unicode&&unicode<0x2223)||
			(0x2262<=unicode&&unicode<0x2264)||
			(0x2268<=unicode&&unicode<0x226a)||
			(0x226c<=unicode&&unicode<0x226e)||
			(0x2284<=unicode&&unicode<0x2286)||
			(0x2590<=unicode&&unicode<0x2592)||
			(0x25b4<=unicode&&unicode<0x25b6)||
			(0x25be<=unicode&&unicode<0x25c0)||
			(0x25c9<=unicode&&unicode<0x25cb)||
			(0x25cc<=unicode&&unicode<0x25ce)||
			(0x2607<=unicode&&unicode<0x2609)||
			(0x303f<=unicode&&unicode<0x3041)||
			(0x3097<=unicode&&unicode<0x3099)||
			(0xffc8<=unicode&&unicode<0xffca)||
			(0xffd0<=unicode&&unicode<0xffd2)||
			(0xffd8<=unicode&&unicode<0xffda)||
			(0x1f12e<=unicode&&unicode<0x1f130)||
			(0x2fffe<=unicode&&unicode<0x30000)||
			(0xffffe<=unicode&&unicode<0x100000)||
			(0x10fffe<=unicode&&unicode<0x110000)||
			0xa9==unicode||
			0xab==unicode||
			0xb5==unicode||
			0xbb==unicode||
			0xe7==unicode||
			0xeb==unicode||
			0xf1==unicode||
			0xfb==unicode||
			0xfd==unicode||
			0x112==unicode||
			0x143==unicode||
			0x14c==unicode||
			0x1cf==unicode||
			0x1d1==unicode||
			0x1d3==unicode||
			0x1d5==unicode||
			0x1d7==unicode||
			0x1d9==unicode||
			0x1db==unicode||
			0x2c8==unicode||
			0x2cc==unicode||
			0x2dc==unicode||
			0x2de==unicode||
			0x3a2==unicode||
			0x3c2==unicode||
			0x450==unicode||
			0x2017==unicode||
			0x2023==unicode||
			0x2031==unicode||
			0x2034==unicode||
			0x2080==unicode||
			0x2104==unicode||
			0x215f==unicode||
			0x21d3==unicode||
			0x2201==unicode||
			0x2210==unicode||
			0x2224==unicode||
			0x2226==unicode||
			0x222d==unicode||
			0x24ea==unicode||
			0x25a2==unicode||
			0x261d==unicode||
			0x2641==unicode||
			0x2662==unicode||
			0x2666==unicode||
			0x266b==unicode||
			0x266e==unicode||
			0x26ce==unicode||
			0x26e2==unicode||
			0x2e9a==unicode||
			0x318f==unicode||
			0x321f==unicode||
			0x32ff==unicode||
			0xfe53==unicode||
			0xfe67==unicode||
			0xffe7==unicode;
	}
	
	//do not contain fullwidth
	template<class T>
	bool is_wide(const T unicode) noexcept
	{
		return
			(0x20000<=unicode&&unicode<0x2fffe)||
			(0x30000<=unicode&&unicode<0x3fffe)||
			(0x4e00<=unicode&&unicode<0xa48d)||
			(0xac00<=unicode&&unicode<0xd7a4)||
			(0x3300<=unicode&&unicode<0x4dc0)||
			(0xf900<=unicode&&unicode<0xfb00)||
			(0x2f00<=unicode&&unicode<0x2fd6)||
			(0x3250<=unicode&&unicode<0x32ff)||
			(0x3099<=unicode&&unicode<0x3100)||
			(0x1100<=unicode&&unicode<0x1160)||
			(0x3131<=unicode&&unicode<0x318f)||
			(0x2e9b<=unicode&&unicode<0x2ef4)||
			(0x3041<=unicode&&unicode<0x3097)||
			(0x3001<=unicode&&unicode<0x303f)||
			(0xa490<=unicode&&unicode<0xa4c7)||
			(0x31f0<=unicode&&unicode<0x321f)||
			(0x3190<=unicode&&unicode<0x31bb)||
			(0x1f210<=unicode&&unicode<0x1f23b)||
			(0x3105<=unicode&&unicode<0x312e)||
			(0x3220<=unicode&&unicode<0x3248)||
			(0x31c0<=unicode&&unicode<0x31e4)||
			(0xfe30<=unicode&&unicode<0xfe53)||
			(0xa960<=unicode&&unicode<0xa97d)||
			(0x2e80<=unicode&&unicode<0x2e9a)||
			(0xfe54<=unicode&&unicode<0xfe67)||
			(0x2ff0<=unicode&&unicode<0x2ffc)||
			(0xfe10<=unicode&&unicode<0xfe1a)||
			(0x1f240<=unicode&&unicode<0x1f249)||
			(0xfe68<=unicode&&unicode<0xfe6c)||
			(0x1f200<=unicode&&unicode<0x1f203)||
			(0x2329<=unicode&&unicode<0x232b)||
			(0x1b000<=unicode&&unicode<0x1b002)||
			(0x1f250<=unicode&&unicode<0x1f252);
	}
}

#endif