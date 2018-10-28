#include"../header/tool/Scope_guard.hpp"
using namespace std;

namespace nTool
{
	Scope_guard::~Scope_guard()
	{
		func_();
	}
}