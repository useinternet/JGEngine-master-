#pragma once

#include"dllExportDefine.h"
#include<Windows.h>
#define USEDIRECTX
namespace JGRC
{
	/*
	EnumClass : EDepthStateType
	@e ZBufferOn  : Z���� On�Ѵ�.
	@e ZBufferOff : Z���� Off�Ѵ�.*/
	enum class CORE_EXPORT EDepthStateType
	{
		ZBufferOn = 0,
		ZBufferOff = 1
	};
	/*
	EnumClass : EBlendStateType
	@e BlendOn  : ���� ���� �����Ѵ�.
	@e BlendOff : ���� ���� �������� �ʴ´�.*/
	enum class CORE_EXPORT EBlendStateType
	{
		BlendOn = 0,
		BlendOff = 1
	};
	/*
	EnumClass : EFillModeType
	@e Solid     : ���� ���̿� ���� ��� ä���.
	@e WireFrame : ���� ���̿� ���� ��� ä���� �ʴ´�.*/
	enum class CORE_EXPORT EFillModeType
	{
		Solid = 0,
		WireFrame = 1
	};


	class CORE_EXPORT JGInitConfig
	{
	public:
		HWND hWnd;
		bool bFullScreen  = false;
		int  ScreenWidth  = 1920;
		int  ScreenHeight = 1080;
		float Fov   = 3.141592f / 4.0f;
		float FarZ  = 1000.0f;
		float NearZ = 0.1f;
	};
	enum class CORE_EXPORT EShaderType
	{
		Pixel,
		Vertex,
		Header,
	};
}