// ConsoleApplication1.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include<iostream>
#include<vector>
int main()
{
	std::vector<int> v = { 0,1,2,3,4,5,6,7 };


	for (auto iter = v.begin(); iter < v.end();)
	{
		if (*iter == 5)
		{
			iter = v.erase(iter);
		}
		else
		{
			++iter;
		}
	}

    return 0;
}

