#include"../header/tool/CScopeGuard.hpp"
using namespace std;

namespace nTool
{
	CScopeGuard::~CScopeGuard()
	{
		func_();
	}
}