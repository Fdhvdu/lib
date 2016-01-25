#include<iterator>
#include<vector>
#include"../header/math/combination.hpp"
#include"../header/tool/show.hpp"

int main()
{
	using namespace std;
	const vector<int> vec{1,2,3,4,5};
	for(const auto &val:nMath::combination(begin(vec),end(vec),3))
		nTool::show(val);
}