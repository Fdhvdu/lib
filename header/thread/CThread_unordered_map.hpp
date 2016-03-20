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
	public:
		using key_type=Key;
		using mapped_type=T;
	private:
		std::unordered_map<key_type,mapped_type,Hash,KeyEqual,Allocator> map_;
		std::mutex mut_;
		template<class Key_typeFwdRef>
		mapped_type& subscript_(Key_typeFwdRef &&key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_[std::forward<Key_typeFwdRef>(key)];
		}
		template<class Key_typeFwdRef,class ... Args>
		bool try_emplace_(Key_typeFwdRef &&key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::forward<Key_typeFwdRef>(key),std::forward<Args>(args)...);
				return true;
			}
			return false;
		}
		template<class Key_typeFwdRef,class Gen>
		bool try_emplace_gen_(Key_typeFwdRef &&key,Gen &&gen)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(!find(key))
			{
				map_.emplace(std::forward<Key_typeFwdRef>(key),std::forward<decltype(gen)>(gen)());
				return true;
			}
			return false;
		}
	public:
		inline mapped_type& at(const key_type &key)
		{
			return map_.at(key);
		}
		inline const mapped_type& at(const key_type &key) const
		{
			return map_.at(key);
		}
		inline bool find(const key_type &key) const
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
		inline bool try_emplace(const key_type &key,Args &&...args)
		{
			return try_emplace_(key,std::forward<Args>(args)...);
		}
		template<class ... Args>
		inline bool try_emplace(key_type &&key,Args &&...args)
		{
			return try_emplace_(std::move(key),std::forward<Args>(args)...);
		}
		template<class Gen>
		inline bool try_emplace_gen(const key_type &key,Gen &&gen)
		{
			return try_emplace_gen_(key,std::forward<decltype(gen)>(gen));
		}
		template<class Gen>
		inline bool try_emplace_gen(key_type &&key,Gen &&gen)
		{
			return try_emplace_gen_(std::move(key),std::forward<decltype(gen)>(gen));
		}
		inline mapped_type& operator[](const key_type &key)
		{
			return subscript_(key);
		}
		inline mapped_type& operator[](key_type &&key)
		{
			return subscript_(std::move(key));
		}
	};
}

#endif