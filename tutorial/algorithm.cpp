#include<algorithm>
#include<vector>
#include"../header/algorithm/algorithm.h"
#include"../header/tool/show.h"
using namespace std;

int main()
{
	vector<int> vec{1,2,1,3,4};
	nTool::show(vec.begin(),unique(vec.begin(),vec.end()));
	nTool::show(vec.begin(),nAlgorithm::unique_without_sort(vec.begin(),vec.end()));

	vector<int> vec2{1,2,1,3,4};
	nTool::show(vec.begin(),nAlgorithm::unique_without_sort(vec.begin(),vec.end(),[](const auto &lhs,const auto &rhs){return lhs%3==rhs%3;}));

	//nAlgorithm::unique_without_sort_thr is thread version, the parameter N means
	//how large distance between begin and end
	//will create a thread to call unique_without_sort
}