#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
/*
�⺻ ���͸��� viewDirection


*/


// ī�޶� ����
/*
���͸���
normal
diff(albedo) <-
specExp, <- ���ݻ籤 exp
specIntensity <- �� ����
*/
/* �ʿ� ����
1. LightDir
2. LightColor
3. 

*/
class CORE_EXPORT Light
{
public:
	jgVec3 AmbientDown;
	jgVec3 AmbientUp;
};