#ifndef PARAMETER_HPP
#define PARAMETER_HPP
#include<any>
#include<iterator>
#include<string>
#include<unordered_map>
#include<utility>

namespace nTool
{
	class Parameter
	{
		std::unordered_map<std::string,std::any> param_;
		template<class T1,class T2>
		bool get_(const std::string &name,T2 &val) const
		{
			const auto iter(param_.find(name));
			if(iter!=std::end(param_))
			{
				if(iter->second.has_value())
					val=std::any_cast<T1>(iter->second);
				return true;
			}
			return false;
		}
	public:
		template<class T1,class T2
			,class=std::enable_if_t<std::negation_v<std::is_same<std::remove_reference_t<T1>,T2>>>
		>
		bool get(const std::string &name,T2 &val) const
		{
			return get_<T1>(name,val);
		}
		template<class T>
		inline bool get(const std::string &name,T &val) const
		{
			return get_<T,T>(name,val);
		}
		inline void set(const std::string &name)
		{
			set(name,std::any{});
		}
		inline void set(std::string &&name)
		{
			set(std::move(name),std::any{});
		}
		template<class T>
		inline void set(const std::string &name,T &&val)
		{
			param_.emplace(name,std::forward<decltype(val)>(val));
		}
		template<class T>
		inline void set(std::string &&name,T &&val)
		{
			param_.emplace(std::move(name),std::forward<decltype(val)>(val));
		}
	};
}

#endif