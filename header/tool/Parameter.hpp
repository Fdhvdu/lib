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
	public:
		template<class T1,class T2>
		bool get(const std::string &name,T2 &val) const
		{
			const auto iter(param_.find(name));
			if(iter!=std::end(param_))
			{
				if(const auto &temp(iter->second);temp.has_value())
					val=std::any_cast<T1>(temp);
				return true;
			}
			return false;
		}
		template<class T>
		inline bool get(const std::string &name,T &val) const
		{
			return get<T,T>(name,val);
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