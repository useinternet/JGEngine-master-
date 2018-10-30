#pragma once
#include"ShaderInformation.h"
namespace JGRC
{
	enum class EShaderType;
	// ��� ���� ���� 
	// ���̴� Ÿ��
	// ��� ���� �� ����
	// map ���·� ����
	// key�� ��� ���� �̸�
	// value�� ��� ����

	// ��� ���� �̸�
	// ��� ���� ���� �̸�
	// ��� ���� ���� ũ��
	// ��� ���� ���� �������� ��

	// ��� stl map
	// key�� ���� �̸����� 
	class CORE_EXPORT CBufferVar
	{
	public:
		std::string       VarName;
		uint              VarSize;
		std::vector<float> vValue;
	};
	class CORE_EXPORT CBuffer
	{
	public:
		std::string CBufferName;
		std::map<uint, CBufferVar> mVars;
	public:
		uint size() const;
	};
	class CORE_EXPORT CBufferInformation : public ShaderInformation
	{
	private:
		std::map<uint, CBuffer> m_mCBuffers;
		//
		uint    m_varNum    = 0;
		uint    m_bufferNum = 0;
		CBuffer m_buffer;
	public:
		CBufferInformation();
		virtual ~CBufferInformation();

		virtual void AnalyzeSentence(std::string& sentence) override;
		const CBuffer& GetCBuffer(const uint idx);
	private:
		bool BlankCheck(const std::string& sentence);
		void CBufferClear();
	};

}
