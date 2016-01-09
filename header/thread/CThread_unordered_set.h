#ifndef CTHREAD_UNORDERED_SET
#define CTHREAD_UNORDERED_SET
#include<functional>
#include<memory>
#include<mutex>
#include<unordered_set>

namespace nThread
{
	template<class Key,class Hash=std::hash<Key>,class KeyEqual=std::equal_to<Key>,class Allocator=std::allocator<Key>>
	class CThread_unordered_set
	{
		std::unordered_set<Key,Hash,KeyEqual,Allocator> set_;
		std::mutex mut_;
	public:
		typedef typename std::unordered_set<Key,Hash,KeyEqual,Allocator>::size_type size_type;
		template<class ... Args>
		bool emplace(Args &&...);
		size_type erase(const Key &);
		inline bool find(const Key &key) const
		{
			return set_.find(key)!=set_.end();
		}
		template<class ... Args>
		bool try_emplace(const Key &,Args &&...);
		template<class ... Args>
		bool try_emplace(Key &&,Args &&...);
	};
}

#include"CThread_unordered_set.cpp"

#endif