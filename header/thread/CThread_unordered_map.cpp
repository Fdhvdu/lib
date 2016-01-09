#include"CThread_unordered_map.h"
#include<utility>

namespace nThread
{
	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::emplace(Args &&...args)
	{
		std::lock_guard<std::mutex> lock{mut_};
		return map_.emplace(std::forward<Args>(args)...).second;
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::try_emplace(const Key &key,Args &&...args)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			map_.emplace(key,std::forward<Args>(args)...);
			return true;
		}
		return false;
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::try_emplace(Key &&key,Args &&...args)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			map_.emplace(std::move(key),std::forward<Args>(args)...);
			return true;
		}
		return false;
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	template<class Generator>
	bool CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::try_emplace_func(const Key &key,Generator &&gen)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			map_.emplace(key,std::forward<Generator>(gen)());
			return true;
		}
		return false;
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	template<class Generator>
	bool CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::try_emplace_func(Key &&key,Generator &&gen)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			map_.emplace(std::move(key),std::forward<Generator>(gen)());
			return true;
		}
		return false;
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	T& CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::operator[](const Key &key)
	{
		std::lock_guard<std::mutex> lock{mut_};
		return map_[key];
	}

	template<class Key,class T,class Hash,class KeyEqual,class Allocator>
	T& CThread_unordered_map<Key,T,Hash,KeyEqual,Allocator>::operator[](Key &&key)
	{
		std::lock_guard<std::mutex> lock{mut_};
		return map_[std::move(key)];
	}
}