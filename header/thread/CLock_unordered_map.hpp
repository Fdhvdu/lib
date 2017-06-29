#ifndef CLOCK_UNORDERED_MAP
#define CLOCK_UNORDERED_MAP
#include<functional>	//equal_to, hash
#include<memory>	//allocator
#include<mutex>
#include<tuple>
#include<unordered_map>
#include<utility>	//forward, move

namespace nThread
{
	template<class Key,class T,class Hash=std::hash<Key>,class KeyEqual=std::equal_to<Key>,class Alloc=std::allocator<std::pair<const Key,T>>>
	class CLock_unordered_map
	{
	public:
		using key_type=Key;
		using mapped_type=T;
	private:
		std::unordered_map<key_type,mapped_type,Hash,KeyEqual,Alloc> map_;
		std::mutex mut_;
		template<class Key_typeFwdRef>
		mapped_type& subscript_(Key_typeFwdRef &&key)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return map_[std::forward<decltype(key)>(key)];
		}
		template<class Key_typeFwdRef,class ... Args>
		bool try_emplace_(Key_typeFwdRef &&key,Args &&...args)
		{
			std::lock_guard<std::mutex> lock{mut_};
			if(find(key))
				return false;
			map_.emplace(std::piecewise_construct,std::forward_as_tuple(std::forward<decltype(key)>(key)),std::forward_as_tuple(std::forward<decltype(args)>(args)...));
			return true;
		}
		template<class Key_typeFwdRef,class Gen>
		bool emplace_if_not_exist_(Key_typeFwdRef &&key,Gen &&gen)
		{
			if(find(key))
				return false;
			map_.emplace(std::piecewise_construct,std::forward_as_tuple(std::forward<decltype(key)>(key)),std::forward_as_tuple(std::forward<decltype(gen)>(gen)()));
			return true;
		}
		template<class Key_typeFwdRef,class Gen>
		bool try_emplace_gen_(Key_typeFwdRef &&key,Gen &&gen)
		{
			std::lock_guard<std::mutex> lock{mut_};
			return emplace_if_not_exist_(std::forward<decltype(key)>(key),std::forward<decltype(gen)>(gen));
		}
		template<class Key_typeFwdRef,class Gen>
		int try_lock_emplace_gen_(Key_typeFwdRef &&key,Gen &&gen)
		{
			std::unique_lock<std::mutex> lock{mut_,std::defer_lock};
			if(lock.try_lock())
				return emplace_if_not_exist_(std::forward<decltype(key)>(key),std::forward<decltype(gen)>(gen));
			return -1;
		}
	public:
		CLock_unordered_map()=default;
		CLock_unordered_map(const CLock_unordered_map &)=delete;
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
			return map_.emplace(std::forward<decltype(args)>(args)...).second;
		}
		inline mapped_type& read(const key_type &key)
		{
			return map_.find(key)->second;
		}
		inline const mapped_type& read(const key_type &key) const
		{
			return map_.find(key)->second;
		}
		template<class ... Args>
		inline bool try_emplace(const key_type &key,Args &&...args)
		{
			return try_emplace_(key,std::forward<decltype(args)>(args)...);
		}
		template<class ... Args>
		inline bool try_emplace(key_type &&key,Args &&...args)
		{
			return try_emplace_(std::move(key),std::forward<decltype(args)>(args)...);
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
		template<class Gen>
		inline int try_lock_emplace_gen(const key_type &key,Gen &&gen)
		{
			return try_lock_emplace_gen_(key,std::forward<decltype(gen)>(gen));
		}
		template<class Gen>
		inline int try_lock_emplace_gen(key_type &&key,Gen &&gen)
		{
			return try_lock_emplace_gen_(std::move(key),std::forward<decltype(gen)>(gen));
		}
		CLock_unordered_map& operator=(const CLock_unordered_map &)=delete;
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
