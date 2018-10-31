#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"
#include"ShaderObject.h"

// D3D10_SHADER_MACRO - > ����   D3DX11CompileFromFile�� �ι�° ����.
// ID3D10Include - > ����  D3DX11CompileFromFile ����° ����.
// ID3DX11ThreadPump -> �Ⱦ��Ű���. �ϴ� ����




// ID3D11ClassLinkage -> �̰͵� ����
namespace JGRC
{
	typedef UINT CompileFlag;
	class InputLayout;

	class ShaderCompiler
	{
	private:
		friend class JGRenderCore;
	public:
		static HWND hWnd;
		static std::string FunctionName;
		static std::string VSVersion;
		static std::string PSVersion;
		static CompileFlag  Flags;
	private:
		static class DirectX* m_pDirectX;
	public:
		bool Success = false;
	public:
		ShaderCompiler() {}
		ShaderCompiler(const std::wstring& hlslPath,ShaderObject* object, InputLayout* layout);
		~ShaderCompiler();
	private:
		bool Compile(const std::wstring& hlslPath, ShaderObject* object, InputLayout* layout);
	private:
		static void ShaderInitialSetting(HWND hWnd, const std::string& functionName, const std::string& shaderVersion,
			const CompileFlag flag);
	};
}


