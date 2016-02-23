#ifndef CID
#define CID
#include<atomic>
#include<cstddef>	//size_t

namespace nTool
{
	class CId
	{
	public:
		using size_type=std::size_t;
	private:
		std::atomic<size_type> id_;
	public:
		//CId::CId() noexcept
		//	:CId{0}{}
		CId() noexcept;
		explicit CId(size_type) noexcept;
		CId(const CId &)=delete;
		inline size_type get() const noexcept
		{
			return id_;
		}
		inline size_type operator++() noexcept
		{
			return ++id_;
		}
		inline size_type operator++(const int) noexcept
		{
			return id_++;
		}
		CId& operator=(const CId &)=delete;
	};
}

#endif