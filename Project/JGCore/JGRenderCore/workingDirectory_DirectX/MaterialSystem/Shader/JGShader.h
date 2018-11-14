#pragma once
#include"HlslEditor.h"
#include"BufferSystem/JGBuffer.h"
namespace JGRC
{
	class CORE_EXPORT JGShader : public JGRCObject
	{
	private:
		friend class ShaderReader;
	public:
		/*
		���̴� ������ ���� ���� */
		static std::string FunctionName;
		static std::string VSVersion;
		static std::string PSVersion;
		static UINT Flags;
	private:
		EShaderType m_ShaderType;
		void* ShaderBuffer;
		/*
		hlsl���� ��� �� ��� ����*/
		std::string m_Path = "None";
		std::vector<JGBuffer*> m_ConstantBuffer;
		/*
		���̴��� �����ϰ� �ִ� ������ */
		InputLayout* m_InputLayout   = nullptr;
		CBufferArray m_CBufferValue;
		Texture2D*   m_Texture2D     = nullptr;
		SamplerState* m_SamplerState = nullptr;
	public:
		JGShader() {}
		virtual ~JGShader();
		/*
		Exp : ���� ���̴��� �����ϰ��ִ� hlsl ��θ� �����´�. */
		const std::string& GetPath() { return m_Path; }
		/*
		Exp : ������ �����´�. ������ ũ�⸦ ��ȯ�Ѵ�. 
		@param VarName : ���� �̸� 
		@param outData : ��� ������ */
		uint  GetParam(const std::string& VarName, real* outData);
		void  SetParam(const std::string& VarName, void* data, const uint Size);
		void  Render();
	private:
		void SetPath(const std::string& path) { m_Path = path; }
		void SetShaderType(const EShaderType type) { m_ShaderType = type; }
		InputLayout*  GetLayout();
		CBufferArray* GetCBufferArray();
		Texture2D*    GetTexture2D();
		SamplerState* GetSamplerState();
		bool RegisterBuffer();
		bool Compile(HWND hWnd);
	};
}
