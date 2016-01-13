#include<vector>
#include"lib/header/math/combination.h"
#include"lib/header/tool/show.h"

int main()
{
	using namespace std;
	const vector<int> vec{1,2,3,4,5};
	for(const auto &val:nMath::combination(begin(vec),end(vec),3))
		nTool::show(val);
}