// JGLibTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include<crtdbg.h>
#include<stdio.h>
#pragma warning(disable : 4996)
//#include<JGCommon/JGCommon.h>
//using namespace std;
//#ifdef _DEBUG
//#pragma comment(lib,"JGCommon/JGCommon_d")
//#else
//#pragma comment(lib,"JGCommon/JGCommon")
//#endif
#include<iostream>
#include<vector>
using namespace std;
#define SIZE 10

int main()
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	int arr[4][4];

	for (int row = 0; row < 4; ++row)
	{
		for (int col = 0; col < 4; ++col)
		{
			arr[row][col] = row * col;
		}
	}
	int col = 0, row = 0;
	for (row = 0; row < 4; ++row)
	{
		for (col = 0; col < 4; ++col)
		{
			printf("%d ", arr[row][col]);
		}
	}
	//



    return 0;
}

