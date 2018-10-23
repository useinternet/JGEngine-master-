// JGLibTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
#include"coordinateMath.h"
using namespace JGCommon;
using namespace std;
int main()
{
	jgVec3 pos = jgVec3(10.0f, 10.0f, -10.0f);
	jgVec3 up = jgVec3(0.0f, 1.0f, 0.0f);
	jgVec3 lookAt = jgVec3(0.0f, 0.0f, 1.0f);

	jgPlane p(up, pos);
	cout << p.dotAssign(jgVec3(1.0f, -30.0f, 3.0f)) << endl;
	p.print_cpp();
    return 0;
}

