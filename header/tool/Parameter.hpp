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
		template<class T>
		bool get(const std::string &name,T &val)
		{
			const auto iter(param_.find(name));
			if(iter!=std::end(param_))
			{
				if(const auto &temp(iter->second);temp.has_value())
					val=std::any_cast<T>(temp);
				return true;
			}
			return false;
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