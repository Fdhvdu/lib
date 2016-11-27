#ifndef CUTF8_READER
#define CUTF8_READER
#include<cstddef>
#include<stdexcept>	//runtime_error
#include<type_traits>	//make_unsigned

namespace nTool
{
	//String_type is std::basic_string
	template<class String_type>
	class CUTF8_reader
	{
		const String_type &hold_;
		std::size_t i_;
		std::size_t size_;
		void calc_size_()
		{
			const auto uc{std::make_unsigned_t<typename String_type::value_type>(hold_[i_])};
			if((127<uc&&uc<192)||(247<uc))
				throw std::runtime_error{"not standard UTF8"};
			if(uc<128)
				size_=1;
			else
				if(uc<224)
					size_=2;
				else
					if(uc<240)
						size_=3;
					else
						if(uc<248)
							size_=4;
		}
	public:
		explicit CUTF8_reader(const String_type &hold)
			:hold_{hold},i_{0}
		{
			calc_size_();
		}
		void next()
		{
			i_+=size_;
			calc_size_();
		}
		inline std::size_t size() const noexcept
		{
			return size_;
		}
		inline String_type str() const
		{
			return hold_.substr(i_,size());
		}
		inline explicit operator bool() const noexcept
		{
			return i_!=hold_.size();
		}
		inline operator String_type() const
		{
			return str();
		}
	};
}

#endif