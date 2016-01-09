#ifndef CTHREAD_UNORDERED_MAP
#define CTHREAD_UNORDERED_MAP
#include<functional>
#include<memory>
#include<mutex>
#include<unordered_map>

namespace nThread
{
	template<class Key,class T,class Hash=std::hash<Key>,class KeyEqual=std::equal_to<Key>,class Allocator=std::allocator<std::pair<const Key,T>>>
	class CThread_unordered_map
	{
		std::unordered_map<Key,T,Hash,KeyEqual,Allocator> map_;
		std::mutex mut_;
	public:
		CThread_unordered_map()=default;
		CThread_unordered_map(const CThread_unordered_map &)=delete;
		inline T& at(const Key &key)
		{
			return map_.at(key);
		}
		inline const T& at(const Key &key) const
		{
			return map_.at(key);
		}
		inline bool find(const Key &key) const
		{
			return map_.find(key)!=map_.end();
		}
		template<class ... Args>
		bool emplace(Args &&...);
		template<class ... Args>
		bool try_emplace(const Key &,Args &&...);
		template<class ... Args>
		bool try_emplace(Key &&,Args &&...);
		template<class Func>
		bool try_emplace_func(const Key &,Func &&);
		template<class Func>
		bool try_emplace_func(Key &&,Func &&);
		CThread_unordered_map& operator=(const CThread_unordered_map &)=delete;
		T& operator[](const Key &);
		T& operator[](Key &&);
	};
}

#include"CThread_unordered_map.cpp"

#endif