#pragma once
#pragma warning (disable : 4996 4005 4251)
#include<JGCommon/JGCommon.h>
#ifdef _DEBUG
#pragma comment(lib,"JGCommon/JGCommon_d.lib")
#else
#pragma comment(lib,"JGCommon/JGCommon.lib")
#endif
#include"dllExportDefine.h"

/* �������� �������  */
//Windows

#include<Windows.h>
#include<wrl.h>
//C++
#include<string>
#include<fstream>
#include<iostream>
//STL
// - �����̳�
#include<vector>
#include<queue>
#include<stack>
#include<unordered_map>
#include<unordered_set>
#include<map>
#include<set>
#include<list>
#include<forward_list>
#include<array>
#include<algorithm>
// - ������
#include<thread>
#include<atomic>
#include<condition_variable>
#include<mutex>
// - �Լ� �� ������ ����
#include<memory>
#include<functional>

// - �ð�
#include<chrono>

//- C���
#include<stdlib.h>



// �ӽ�
namespace Game
{
	static JGPath::Path path("../../../../../../../Contents");
	//static JGPath::Path path("../../Contents");
}



