#pragma once
#include"HlslEditor.h"
#include"BufferSystem/JGBuffer.h"
namespace JGRC
{
	class CORE_EXPORT JGShader : public JGRCObject
	{
		typedef std::unordered_map<std::string, ID3D11ShaderResourceView*> unSRVArray;
		typedef std::pair<std::string, ID3D11ShaderResourceView*>          unSRVPair;
	private:
		static uint RefCount;
		static unSRVArray m_umSRVs;
		friend class ShaderReader;
	public:
		/*
		���̴� ������ ���� ���� */
		static std::string FunctionName;
		static std::string VSVersion;
		static std::string PSVersion;
		static UINT Flags;
	private:
		JGShader(const JGShader& copy) = delete;
		JGShader(JGShader&& shader)    = delete;
		JGShader& operator=(const JGShader& copy)    = delete;
		JGShader& operator=(const JGShader&& shader) = delete;
	private:
		EShaderType m_ShaderType;
		void* ShaderBuffer;
		/*
		hlsl���� ��� �� ��� ����*/
		std::string m_Path = "None";
		std::vector<JGBuffer*> m_ConstantBuffer;
		/*
		���̴��� �����ϰ� �ִ� ������ */
		InputLayout*  m_InputLayout   = nullptr;
		CBufferArray  m_CBufferValue;
		Texture2D*    m_Texture2D     = nullptr;
		SamplerState* m_SamplerState  = nullptr;
	public:
		JGShader() { RefCount++; }
		virtual ~JGShader();
		bool  AddTexture(const std::string& name,const std::string& path);
		bool  AddTexture(ID3D11ShaderResourceView* srv);
		/*
		Exp : ���� ���̴��� �����ϰ��ִ� hlsl ��θ� �����´�. */
		const std::string& GetPath() { return m_Path; }
		/*
		Exp : ������ �����´�. ������ ũ�⸦ ��ȯ�Ѵ�. 
		@param VarName : ���� �̸� 
		@param outData : ��� ������ */
		uint  GetParam(const std::string& VarName, real* outData);
		/*
		Exp :  �������� �����Ѵ�. 
		@param varName : ���� �̸� 
		@param data    : ���� ������ 
		@param Count   : ���� ������ ����(real ����) */
		void  SetParam(const std::string& VarName, void* data, const uint Count);
		/*
		Exp : ������ */
		void  Render();

		EShaderType GetType() { return m_ShaderType; }
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
