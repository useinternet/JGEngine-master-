// JGLibTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include"coordinateMath.h"
using namespace JGCommon;
int main()
{
	JGCommon::jgPlane p(jgVec3(1.0f, 3.0f, 2.0f), jgVec3(5.0f, 2.0f, 0.0f), jgVec3(3.0f, -1.0f, 6.0f));
	p.print_cpp();
    return 0;
}

