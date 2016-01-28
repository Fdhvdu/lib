#ifndef CTHREAD_UNORDERED_MAP
#define CTHREAD_UNORDERED_MAP
#include<functional>	//equal_to, hash
#include<memory>	//allocator
#include<mutex>
#include<unordered_map>
#include<utility>	//forward, move

namespace nThread
{
	template<class Key,class T,class Hash=std::hash<Key>,class KeyEqual=std::equal_to<Key>,class Allocator=std::allocator<std::pair<const Key,T>>>
	class CThread_unordered_map
	{
		std::unordered_map<Key,T,Hash,KeyEqual,Allocator> map_;
		std::mutex mut_;
		template<class KeyFwdRef>
		T& subscript_(KeyFwdRef &&key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_[std::forward<KeyFwdRef>(key)];
		}
		template<class KeyFwdRef,class ... Args>
		bool try_emplace_(KeyFwdRef &&key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::forward<KeyFwdRef>(key),std::forward<Args>(args)...);
				return true;
			}
			return false;
		}
		template<class KeyFwdRef,class Generator>
		bool try_emplace_func_(KeyFwdRef &&key,const Generator gen)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::forward<KeyFwdRef>(key),gen());
				return true;
			}
			return false;
		}
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
		bool emplace(Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_.emplace(std::forward<Args>(args)...).second;
		}
		template<class ... Args>
		inline bool try_emplace(const Key &key,Args &&...args)
		{
			return try_emplace_(key,std::forward<Args>(args)...);
		}
		template<class ... Args>
		inline bool try_emplace(Key &&key,Args &&...args)
		{
			return try_emplace_(std::move(key),std::forward<Args>(args)...);
		}
		template<class Generator>
		inline bool try_emplace_func(const Key &key,const Generator gen)
		{
			return try_emplace_func_(key,gen);
		}
		template<class Generator>
		inline bool try_emplace_func(Key &&key,const Generator gen)
		{
			return try_emplace_func_(std::move(key),gen);
		}
		CThread_unordered_map& operator=(const CThread_unordered_map &)=delete;
		inline T& operator[](const Key &key)
		{
			return subscript_(key);
		}
		inline T& operator[](Key &&key)
		{
			return subscript_(std::move(key));
		}
	};
}

#endif