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
		std::vector<float> vValue;
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
		void  getData(std::vector<float>* dataArray);
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
		/*
		Exp : ��� ���� ������ ��� �´�. 
		@param idx : ���� �ε��� */
		CBuffer* GetCBuffer(const uint idx);


		float*   GetParam(const std::string& paramName);
		void     SetParam(const std::string& paramName, void* value);
		uint     GetParamSize(const std::string& paramName);
		uint     GetCBufferCount();
	private:
		bool BlankCheck(const std::string& sentence);
		void CBufferClear();
		CBufferVar* FindVar(const std::string& name);
	};

}
