#include<algorithm>
#include<iterator>
#include<vector>
#include"../header/algorithm/algorithm.h"
#include"../header/tool/show.h"

void multiply_tutorial()
{
	using namespace std;

	vector<int> vec{1,2,3};
	vector<int> vec2{6,5,4};
	vector<int> vec3;
	nAlgorithm::multiply(begin(vec),end(vec),begin(vec2),end(vec2),back_inserter(vec3),[](const int lhs,const int rhs){return lhs+rhs;});
	nTool::show(vec3);
}

void unique_without_sort_tutorial()
{
	using namespace std;

	vector<int> vec{1,2,1,3,4};
	nTool::show(begin(vec),unique(begin(vec),end(vec)));
	nTool::show(begin(vec),nAlgorithm::unique_without_sort(begin(vec),end(vec)));

	vector<int> vec2{1,2,1,3,4};
	nTool::show(begin(vec),nAlgorithm::unique_without_sort(begin(vec),end(vec),[](const auto &lhs,const auto &rhs){return lhs%3==rhs%3;}));

	//nAlgorithm::unique_without_sort_thr is a thread version, the parameter N means
	//how large distance between begin and end
	//will create a thread to call unique_without_sort
}

int main()
{
	multiply_tutorial();
	unique_without_sort_tutorial();
}