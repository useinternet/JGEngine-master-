#pragma once
#include<Windows.h>
#include"../GameLogic/Sample.h"


#pragma comment(lib,"GameLogic.lib")
#define BridgeExport __declspec(dllexport)


// x64 x32 ���� ���� �����
// x64 x32 �����/������(�������ϸ���) ��������� ����� ���� ���Ϸ�..
// x64 x32 �����/������(�������ϸ���) ��������� ����� ���� ���Ϸ�..

extern "C"
{
	BridgeExport SampleClass* g_sample = nullptr;
	BridgeExport void InitEngine(HINSTANCE Instance, HWND hWnd, int ScreenWidth, int ScreenHeight);

	BridgeExport void Start();

	BridgeExport void End();

	BridgeExport void EG2ED_SendCommand(const char* Command, void* Data, unsigned int Size);
	BridgeExport void ED2EG_SendCommand(const char* Command, void* Data, unsigned int Size);
}