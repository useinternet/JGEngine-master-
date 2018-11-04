#pragma once
#include"ShaderInformation.h"
#include"MaterialSystem/Data/ShaderData.h"
namespace JGRC
{

	/*
	Exp : hlsl ���Ϸ� ���� ���÷� ������ �ص��Ѵ�. */
	class CORE_EXPORT SamplerInformation : public ShaderInformation
	{
	private: 
		
		uint m_SamplerCount = 0;                            // ���÷� ��ȣ�� �ű�� ���� ����
		std::vector<D3D11_SAMPLER_DESC> m_vSamplerDescs;    // ���÷� Desc �迭
		std::vector<ESamplerMode>       m_vModes;           // ���÷� ��� �迭
		D3D11_SAMPLER_DESC m_SamplerDesc;                   // ���÷��� �ص��ϰ� �����ϱ� ���� ����
		ESamplerMode       m_Mode;                          // ���÷��� �ص��ϰ� �����ϱ� ���� ����
	public:
		SamplerInformation();

		virtual ~SamplerInformation();
		virtual void AnalyzeSentence(std::string& sentence) override;
		virtual bool Decryptable(const std::string& sentence) override;
		virtual void WriteShaderData(std::ofstream& fout) override;
	private:
		void SamplerCustomMode(std::string& sentence);
		void SamplerDefault_WrapMode();
		void SamplerDefault_ClampMode();
	};
}
