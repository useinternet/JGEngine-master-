#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"

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
	class CORE_EXPORT Layout
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
	class CORE_EXPORT LayoutInformation
	{
	private:
		static const std::string INPUTLAYOUT;
		static const std::string INPUTSLOT;
		static const std::string INSTNACE;
		static const std::string FLOAT;
		static const std::string FLOAT2;
		static const std::string FLOAT3;
		static const std::string FLOAT4;
	private:
		std::vector<Layout> m_vLayout;
		bool   m_bIsInstance = false;
		bool   m_bIsProgressing = false;
		uint   inputSlot = 0;
	public:
		LayoutInformation();
		~LayoutInformation();
		bool IsProgressing() const;
		void AnalyzeSentence(std::string& sentence);
	private:
		uint ExtractionBracketNumber(const std::string& s);
	};

}
