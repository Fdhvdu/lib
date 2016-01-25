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
		bool try_emplace(const Key &key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(key,std::forward<Args>(args)...);
				return true;
			}
			return false;
		}
		template<class ... Args>
		bool try_emplace(Key &&key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::move(key),std::forward<Args>(args)...);
				return true;
			}
			return false;
		}
		template<class Generator>
		bool try_emplace_func(const Key &key,Generator &&gen)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(key,std::forward<Generator>(gen)());
				return true;
			}
			return false;
		}
		template<class Generator>
		bool try_emplace_func(Key &&key,Generator &&gen)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::move(key),std::forward<Generator>(gen)());
				return true;
			}
			return false;
		}
		CThread_unordered_map& operator=(const CThread_unordered_map &)=delete;
		T& operator[](const Key &key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_[key];
		}
		T& operator[](Key &&key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_[std::move(key)];
		}
	};
}

#endif