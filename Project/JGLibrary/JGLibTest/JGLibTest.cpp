// JGLibTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include<crtdbg.h>
#include<stdio.h>
//#include<JGCommon/JGCommon.h>
//using namespace std;
//#ifdef _DEBUG
//#pragma comment(lib,"JGCommon/JGCommon_d")
//#else
//#pragma comment(lib,"JGCommon/JGCommon")
//#endif
#include<iostream>
using namespace std;

template<typename Type>
Type add(Type a, Type b)
{
	Type n = a + b;
	return n;
}
int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);






    return 0;
}

