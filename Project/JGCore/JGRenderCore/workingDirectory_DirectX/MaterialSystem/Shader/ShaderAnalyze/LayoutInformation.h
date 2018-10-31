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
	/*
	Exp : hlsl ������ �ص��Ͽ� �Է� ���̾ƿ� ������ �����´�. */
	class CORE_EXPORT LayoutInformation : public ShaderInformation
	{
		class LayoutDesc
		{
		public:
			std::string SemanticName;
			UINT SemanticIndex;
			DXGI_FORMAT Format;
			UINT InputSlot;
			UINT AlignedByteOffset;
			D3D11_INPUT_CLASSIFICATION InputSlotClass;
			UINT InstanceDataStepRate;
		};
	private:
		std::vector<LayoutDesc> m_vLayout;  // ���̾ƿ� ��� �迭
		bool   m_bIsInstance = false;                     // �ν��Ͻ� ������ �����ϴ��� ���� 
		uint   m_InstanceDataSR = 1;                      // �ν��Ͻ� DataSetpRate �� �����ϱ� ���� ����
		//
		uint   m_inputSlot = 0;                           //  ���� �Է� ����
		uint   m_pvInputSlot = 0;                         //  �� �Է� ����
		//
		uint   m_accOffset = 0;                           // ���� ������
	public:
		LayoutInformation();
		virtual ~LayoutInformation();
		virtual void AnalyzeSentence(std::string& sentence) override;
		virtual bool Decryptable(const std::string& sentence) override;

		void MakeInputLayoutArray(class InputLayout* layout);
		/*
		Exp : ��� ����� �����´�. */
		uint Size();
	};

}
