#include"CThread_unordered_set.h"
#include<utility>

namespace nThread
{
	template<class Key,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_set<Key,Hash,KeyEqual,Allocator>::emplace(Args &&...args)
	{
		std::unique_lock<std::mutex> lock{mut_};
		return set_.emplace(std::forward<Args>(args)...).second;
	}

	template<class Key,class Hash,class KeyEqual,class Allocator>
	typename CThread_unordered_set<Key,Hash,KeyEqual,Allocator>::size_type CThread_unordered_set<Key,Hash,KeyEqual,Allocator>::erase(const Key &key)
	{
		std::unique_lock<std::mutex> lock{mut_};
		return set_.erase(key);
	}

	template<class Key,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_set<Key,Hash,KeyEqual,Allocator>::try_emplace(const Key &key,Args &&...args)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			set_.emplace(key,std::forward<Args>(args)...);
			return true;
		}
		return false;
	}

	template<class Key,class Hash,class KeyEqual,class Allocator>
	template<class ... Args>
	bool CThread_unordered_set<Key,Hash,KeyEqual,Allocator>::try_emplace(Key &&key,Args &&...args)
	{
		std::lock_guard<std::mutex> lock{mut_};
		if(!find(key))
		{
			set_.emplace(std::move(key),std::forward<Args>(args)...);
			return true;
		}
		return false;
	}
}