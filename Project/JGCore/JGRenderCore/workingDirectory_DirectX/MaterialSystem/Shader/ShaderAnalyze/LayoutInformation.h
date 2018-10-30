#pragma once
#include"ShaderInformation.h"

// ���ؽ� ���̾ƿ��� #define���� ����
// ���̾ƿ� ����
// ù��° �̸� ���� ����
// �ø�ƽ ��ȣ ����
// ���� ���� -> float , float2, float3, float4  �̷��� 4��
// �Է� ���� <- �̰Ŵ� ��Ÿ �����ͷ� �����ؼ� ����..
// �������� ����������
// ���ؽ� ��������
// �ν��Ͻ� 0,


namespace JGRC
{

	class CORE_EXPORT LayoutInformation : public ShaderInformation
	{
	private:
		class Layout
		{
		public:
			std::string sementicName;
			uint sementicNum;
			DXGI_FORMAT format;
			uint inputSlot;
			uint offset;
			D3D11_INPUT_CLASSIFICATION inputClass;
			uint InstanceDataStepRate;
		};
	private:
		std::vector<D3D11_INPUT_ELEMENT_DESC> m_vLayout;
		bool   m_bIsInstance = false;
		uint   m_InstanceDataSR = 1;
		//
		uint   m_inputSlot = 0;
		uint   m_pvInputSlot = 0;
		//
		uint   m_accOffset = 0;
	public:
		LayoutInformation();
		virtual ~LayoutInformation();
		virtual void AnalyzeSentence(std::string& sentence) override;
		D3D11_INPUT_ELEMENT_DESC& GetDesc(const uint idx);
		D3D11_INPUT_ELEMENT_DESC* GetDescAddress();
		uint Size();
	};

}
