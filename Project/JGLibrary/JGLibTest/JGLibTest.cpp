// JGLibTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include<crtdbg.h>
#include<stdio.h>
#include<JGCommon/JGCommon.h>
using namespace std;
#ifdef _DEBUG
#pragma comment(lib,"JGCommon/JGCommon_d")
#else
#pragma comment(lib,"JGCommon/JGCommon")
#endif
/*
BOOST_LOG_TRIVIAL(trace) << "A trace severity message";
BOOST_LOG_TRIVIAL(debug) << "A debug severity message"; 
BOOST_LOG_TRIVIAL(info) << "An informational severity message"; 
BOOST_LOG_TRIVIAL(warning) << "A warning severity message"; 
BOOST_LOG_TRIVIAL(error) << "An error severity message"; 
BOOST_LOG_TRIVIAL(fatal) << "A fatal severity message";


*/
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//BOOST_LOG_TRIVIAL(trace) << "A trace severity message";

    return 0;
}

