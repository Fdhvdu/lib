#ifndef CLOCK_UNORDERED_SET
#define CLOCK_UNORDERED_SET
#include<functional>	//equal_to, hash
#include<memory>	//allocator
#include<mutex>
#include<unordered_set>
#include<utility>	//forward, move

namespace nThread
{
	template<class Key,class Hash=std::hash<Key>,class KeyEqual=std::equal_to<Key>,class Alloc=std::allocator<Key>>
	class CLock_unordered_set
	{
		std::unordered_set<Key,Hash,KeyEqual,Alloc> set_;
		std::mutex mut_;
		template<class KeyFwdRef,class ... Args>
		bool try_emplace_(KeyFwdRef &&key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(find(key))
				return false;
			set_.emplace(std::forward<decltype(key)>(key),std::forward<decltype(args)>(args)...);
			return true;
		}
	public:
		using size_type=typename std::unordered_set<Key,Hash,KeyEqual,Alloc>::size_type;
		CLock_unordered_set()=default;
		CLock_unordered_set(const CLock_unordered_set &)=delete;
		template<class ... Args>
		bool emplace(Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return set_.emplace(std::forward<decltype(args)>(args)...).second;
		}
		size_type erase(const Key &key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return set_.erase(key);
		}
		inline bool find(const Key &key) const
		{
			return set_.find(key)!=set_.end();
		}
		template<class ... Args>
		inline bool try_emplace(const Key &key,Args &&...args)
		{
			return try_emplace_(key,std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		inline bool try_emplace(Key &&key,Args &&...args)
		{
			return try_emplace_(std::move(key),std::forward<decltype(args)>(args)...);
		}
		CLock_unordered_set& operator=(const CLock_unordered_set &)=delete;
	};
}

#endif