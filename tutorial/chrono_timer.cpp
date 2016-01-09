#include<iostream>
#include"../header/tool/CChrono_timer.h"
using namespace std;

void print_timer(const nTool::CChrono_timer &timer)
{
	cout<<timer.duration_hours()<<endl;
	cout<<timer.duration_minutes()%60<<endl;
	cout<<timer.duration_seconds()%60<<endl;
	cout<<timer.duration_milliseconds()%1000<<endl;
	cout<<timer.duration_microseconds()%1000<<endl;
	cout<<timer.duration_nanoseconds()%1000<<endl;
}

int main()
{
	nTool::CChrono_timer timer;
	timer.start();
	for(auto i{1000000000};--i;)
		;
	timer.stop();
	print_timer(timer);
	timer.start();
	for(auto i{2000000000};--i;)
		;
	timer.stop();
	print_timer(timer);
}