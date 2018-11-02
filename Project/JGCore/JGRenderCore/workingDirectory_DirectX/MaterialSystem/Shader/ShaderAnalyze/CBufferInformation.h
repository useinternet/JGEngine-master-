#pragma once
#include"ShaderInformation.h"
namespace JGRC
{
	enum class EShaderType;


	/*
	Exp : CBuffer�ȿ� ����ִ� ���� */
	class CORE_EXPORT CBufferVar
	{
	public:
		std::string       VarName;
		uint              VarSize;
		uint              VarCount;
	};
	/*
	Exp : ��� ���� */
	class CORE_EXPORT CBuffer
	{
	public:
		std::string CBufferName;
		std::map<uint, CBufferVar> mVars;
	public:
		uint  size() const;
	};
	class CORE_EXPORT CBufferInformation : public ShaderInformation
	{
	private:
		std::map<uint, CBuffer> m_mCBuffers;    // ��� ���� �迭
		// 
		uint    m_varNum    = 0;                // ���� ��ȣ
		uint    m_bufferNum = 0;                // ���� ��ȣ
		CBuffer m_buffer;                       // ������ ��� ���� ����
	public:
		CBufferInformation();
		virtual ~CBufferInformation();

		virtual void AnalyzeSentence(std::string& sentence) override;
		virtual bool Decryptable(const std::string& sentence) override;
		virtual void WriteShaderData(std::ofstream& fout) override;
	private:
		bool BlankCheck(const std::string& sentence);
		void CBufferClear();
		CBufferVar* FindVar(const std::string& name);
	};

}
