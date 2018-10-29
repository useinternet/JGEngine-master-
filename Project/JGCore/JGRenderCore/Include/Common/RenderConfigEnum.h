#pragma once
#include"Common/dllExportDefine.h"

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

}