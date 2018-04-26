#ifndef FIXED_VECTOR_HPP
#define FIXED_VECTOR_HPP
#include<algorithm>
#include<type_traits>
#include<utility>

namespace nContainer
{
	template<class T,auto N>
	class Fixed_vector
	{
	public:
		using value_type=T;
		using size_type=std::remove_cv_t<decltype(N)>;
		using pointer=T*;
		using const_pointer=const T*;
		using reference=T&;
		using const_reference=const T&;
		using iterator=T*;
		using const_iterator=const T*;
	private:
		value_type data_[N];
		pointer end_;
	public:
		constexpr Fixed_vector() noexcept(std::is_nothrow_default_constructible_v<T>)
			:end_(data_){}
		Fixed_vector(const Fixed_vector &rhs)
			:Fixed_vector(rhs.begin(),rhs.end()){}
		Fixed_vector(Fixed_vector &&rhs)
		{
			end_=std::move(rhs.begin(),rhs.end(),data_);
		}
		template<class InIter>
		constexpr Fixed_vector(InIter begin,InIter end)
		{
			end_=std::copy(begin,end,data_);
		}
		Fixed_vector(const Fixed_vector &rhs)=delete;
		constexpr reference back() noexcept
		{
			return *(end_-1);
		}
		constexpr const_reference back() const noexcept
		{
			return *(end_-1);
		}
		constexpr iterator begin() noexcept
		{
			return data_;
		}
		constexpr const_iterator begin() const noexcept
		{
			return data_;
		}
		constexpr size_type capacity() const noexcept
		{
			return N;
		}
		constexpr void clear() noexcept
		{
			end_=data_;
		}
		constexpr iterator end() noexcept
		{
			return end_;
		}
		constexpr const_iterator end() const noexcept
		{
			return end_;
		}
		constexpr bool empty() const noexcept
		{
			return data_==end_;
		}
		void erase(const const_iterator pos)
		{
			std::move(pos+1,end_,pos);
		}
		constexpr pointer data() noexcept
		{
			return data_;
		}
		constexpr const_pointer data() const noexcept
		{
			return data_;
		}
		constexpr reference front() noexcept
		{
			return data_[0];
		}
		constexpr const_reference front() const noexcept
		{
			return data_[0];
		}
		constexpr void pop_back() noexcept
		{
			--end_;
		}
		constexpr void push_back(const value_type &val) noexcept(std::is_nothrow_copy_assignable_v<value_type>)
		{
			*(end_++)=val;
		}
		constexpr void push_back(value_type &&val) noexcept(std::is_nothrow_move_assignable_v<value_type>)
		{
			*(end_++)=std::move(val);
		}
		constexpr void resize(const size_type new_size) noexcept
		{
			end_=data_+new_size;
		}
		constexpr size_type size() const noexcept
		{
			return end_-data_;
		}
		constexpr reference operator[](const size_type pos)
		{
			return data_[pos];
		}
		constexpr const_reference operator[](const size_type pos) const
		{
			return data_[pos];
		}
		Fixed_vector& operator=(const Fixed_vector &rhs)=delete;
	};
}

#endif