#pragma once
#include"Common/dllExportDefine.h"
#include<Windows.h>
namespace JGRC
{
	class DirectX;
	class JGInitConfig;
	class TestShader;
	class TestModel;
	class CORE_EXPORT JGRenderCore
	{
	private:
		DirectX* m_DX;
		TestShader* shader;
		TestModel*  model;
	public:
		JGRenderCore();
		virtual ~JGRenderCore();

		void Init(HWND hWnd);
		void Draw();
	private:
		void modelDraw();
	};
}


