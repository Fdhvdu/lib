#ifndef CONTAINER_ADAPTOR_HPP
#define CONTAINER_ADAPTOR_HPP

namespace nTool
{
	template<class T>
	struct Container_adaptor:public T
	{
		inline typename T::container_type& get_c() noexcept
		{
			return this->c;
		}
		inline const typename T::container_type& get_c() const noexcept
		{
			return this->c;
		}
	};
}

#endif