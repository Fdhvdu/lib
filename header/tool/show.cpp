#include"show.h"
#include<algorithm>

namespace nTool
{
	template<class InIter,class T,class UnaryOp>
	void show(InIter begin,InIter end,std::ostream &os,const T &delim,const UnaryOp pred)
	{
		using namespace std;
		if(begin!=end)
		{
			os<<pred(*begin);
			for_each(++begin,end,[&,pred](const typename iterator_traits<InIter>::value_type &val){os<<delim<<op(val);});
			os<<endl;
		}
	}
}