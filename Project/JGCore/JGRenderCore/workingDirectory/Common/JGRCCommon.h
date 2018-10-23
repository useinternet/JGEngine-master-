#pragma once

#ifdef JGRENDERCORE_EXPORTS 
#define CORE_EXPORT __declspec(dllexport)
#else
#define CORE_EXPORT __declspec(dllimport)
#endif


#pragma warning (disable : 4996 4005 4251)

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

// ���̷�Ʈ X
#ifdef USEDIRECTX
#define DIRECTINPUT_VERSION 0x0800
#include"DirectX/Include/D3D11.h"
#include"DirectX/Include/DXGI.h"
#include"DirectX/Include/D3Dcompiler.h"
#include"DirectX/Include/D3Dcommon.h"
#include"DirectX/Include/D3D11Shader.h"
#include"DirectX/Include/dinput.h"
#include"DirectX/Include/D3DX11async.h"
#include"DirectX/Include/d3dx9core.h"
// lib
#ifdef WIN64
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3d11.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/dxgi.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3dcompiler.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/dxguid.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/dinput8.lib")
#ifdef _DEBUG
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3dx11d.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3dx10d.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/D3dx9d.lib")

#else
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3dx11.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/d3dx10.lib")
#pragma comment(lib,"workingDirectory/DirectX/Lib/x64/D3dx9.lib")
#endif
#endif
#endif

