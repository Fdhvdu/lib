#include"CTask.h"
#include<functional>	//bind
#include<utility>	//forward

namespace nThread
{
	template<class Ret>
	template<class Func,class ... Args>
	CTask<Ret>::CTask(Func &&func,Args &&...args)
		:task_{std::bind(std::forward<Func>(func),std::forward<Args>(args)...)},fut_{task_.get_future()}{}
}