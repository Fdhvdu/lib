#include<iterator>
#include<utility>
#include<string>
#include<vector>
#include"../header/algorithm/exchange_endpoint_by_swapping.h"
#include"../header/tool/show.h"

int main()
{
	using namespace std;
	vector<int> vec{1,2,3,4};
	for(const auto &val:nAlgorithm::exchange_endpoint_by_swapping(begin(vec),end(vec),[](const int lhs,const int rhs){return pair<int,int>{lhs,rhs};}))
		nTool::show(begin(val),end(val),cout,"  ",[](const pair<int,int> &val){return to_string(val.first)+','+to_string(val.second);});
}