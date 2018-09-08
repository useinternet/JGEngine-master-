#pragma once

/*   ��� ���� ���� */
#pragma warning(disable: 4005)
#include"../../ManagementFiles/Include/DirectX/D3D11.h"
#include"../../ManagementFiles/Include/DirectX/DXGI.h"
#include"../../ManagementFiles/Include/DirectX/D3DCompiler.h"
#include"../../ManagementFiles/Include/DirectX/D3DX10math.h"
#include"../../ManagementFiles/Include/DirectX/D3DCommon.h"
#include"../../ManagementFiles/Include/DirectX/D3DX9Core.h"
#include"../../ManagementFiles/Include/DirectX/D3D11Shader.h"
#include"../../ManagementFiles/Include/DirectX/dinput.h"
#include"../../ManagementFiles/Include/DirectX/d3dx11async.h"
#include"../../ManagementFiles/Include/FMODInclude_LowLevel/fmod.h"
#include"../../ManagementFiles/Include/FMODInclude_LowLevel/fmod.hpp"
#include"../../ManagementFiles/Include/Box2D/Box2D.h"

#pragma warning(default: 4005)

/* ���̺귯�� ��ũ */
#ifdef _WIN64
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3d11.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/dxgi.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3dcompiler.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/dxguid.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/dinput8.lib")
#ifdef _DEBUG  
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3dx11d.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3dx10d.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/D3dx9d.lib")
#pragma comment(lib,"../ManagementFiles/Library/Box2D/Box2Dd.lib")
#else          
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3dx11.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/d3dx10.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win64/D3dx9.lib")
#pragma comment(lib,"../ManagementFiles/Library/Box2D/Box2D.lib")
#endif

#else
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3d11.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/dxgi.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3dcompiler.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/dxguid.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/dinput8.lib")
#ifdef _DEBUG   
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3dx11d.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3dx10d.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/D3dx9d.lib")
#pragma comment(lib,"../ManagementFiles/Library/Box2D/Box2Dd.lib")
#else          
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3dx11.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/d3dx10.lib")
#pragma comment(lib,"../ManagementFiles/Library/DirectX/Win32/D3dx9.lib")
#pragma comment(lib,"../ManagementFiles/Library/Box2D/Box2D.lib")
#endif

#endif
// ���� FOMD ���̺귯�� �ҷ����� 
#pragma comment(lib,"../ManagementFiles/Library/FMODLib_LowLevel/fmod64_vc.lib")

/* ��� ���� */
#pragma warning(disable: 4251)


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


// ����� ����
#include"EngineMacro.h"


// �ͽ���Ʈ ����
#ifdef JGENGINECORE_EXPORTS
#define ENGINE_EXPORT __declspec(dllexport)
#else
#define ENGINE_EXPORT __declspec(dllimport)
#endif
