#pragma once
#include<Windows.h>
#ifdef ENGINEBRIDGE_EXPORTS
#define BRIDGE_EXPORT __declspec(dllexport)
#else
#define BRIDGE_EXPORT __declspec(dllimport)
#endif
#ifdef _DEBUG
#pragma comment(lib,"JGMainCore_d.lib")
#else
#pragma comment(lib,"JGMainCore.lib")
#endif



// x64 x32 ���� ���� �����
// x64 x32 �����/������(�������ϸ���) ��������� ����� ���� ���Ϸ�..
// x64 x32 �����/������(�������ϸ���) ��������� ����� ���� ���Ϸ�..

extern "C"
{
	BRIDGE_EXPORT class JGMainCore* g_Engine = nullptr;
	BRIDGE_EXPORT void InitEngine(HWND hWnd);
	
	BRIDGE_EXPORT void Run();

	BRIDGE_EXPORT void End();

	BRIDGE_EXPORT void EG2ED_SendCommand(const char* Command, void* Data, unsigned int Size);
	BRIDGE_EXPORT void ED2EG_SendCommand(const char* Command, void* Data, unsigned int Size);
}