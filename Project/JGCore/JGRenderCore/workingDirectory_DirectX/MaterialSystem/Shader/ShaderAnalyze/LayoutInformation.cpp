#include"LayoutInformation.h"
#include"MaterialSystem/Shader/ShaderTool/InputLayout.h"
using namespace std;
using namespace JGRC;

LayoutInformation::LayoutInformation()
{


}
LayoutInformation::~LayoutInformation()
{


}
void LayoutInformation::AnalyzeSentence(string& sentence)
{
	if (!Decryptable(sentence) || StringUtil::FindString(sentence, "#define"))
	{
		return;
	}
	Start();
	// �ΰ����� ������ �ǳʶٱ�
	if (StringUtil::FindString(sentence, "struct") || sentence == "{" || sentence == "};")
	{
		if (sentence == "};")
			Stop(); 
		return;
	}
	// �Է� ���� �߰�
	else if (StringUtil::FindString(sentence, hlslType::INPUTSLOT.c_str()))
	{
		m_bIsInstance = false;
		m_inputSlot = ExtractionBracketNumber(sentence);
		if (m_inputSlot != m_pvInputSlot)
		{
			m_accOffset = 0;
		}
	}
	// �ν��Ͻ� ���� �Է� 
	else if (StringUtil::FindString(sentence, hlslType::INSTNACE.c_str()))
	{
		m_bIsInstance = true;
		m_InstanceDataSR = ExtractionBracketNumber(sentence);
	}
	else if (StringUtil::FindString(sentence,":"))
	{
		LayoutDesc ly;
		// �Է� ���� / ������ / �Է� Ŭ���� ����
		ly.InputSlot = (UINT)m_inputSlot;
		ly.AlignedByteOffset = (UINT)m_accOffset;
		// �ν��Ͻ��� ���ؽ� �Է� ���̾ƿ��� ���� ���� �ٸ��� �Է�
		if (m_bIsInstance)
		{
			ly.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			ly.InstanceDataStepRate = (UINT)m_InstanceDataSR;
			m_bIsInstance = false;
		}
		else
		{
			ly.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
			ly.InstanceDataStepRate = 0;
		}
		// ������ ���� �ޱ�
		if (StringUtil::FindString(sentence, hlslType::FLOAT.c_str()))
		{
			ly.Format = DXGI_FORMAT_R32_FLOAT;
			m_accOffset += sizeof(float);
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT2.c_str()))
		{
			ly.Format = DXGI_FORMAT_R32G32_FLOAT;
			m_accOffset += sizeof(float) * 2;
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT3.c_str()))
		{
			ly.Format = DXGI_FORMAT_R32G32B32_FLOAT;
			m_accOffset += sizeof(float) * 3;
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT4.c_str()))
		{
			ly.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			m_accOffset += sizeof(float) * 4;
		}
		// �ø�ƽ �̸� ���
		uint len = sentence.length();
		uint pos = sentence.find(':');
		string smName = sentence.substr(pos + 1, len - 1);
		StringUtil::DelChar(smName,';');
		StringUtil::DelBlank(smName);
		
		// �ڿ� ���� Ȯ��
		len = smName.length();
		if (isdigit(smName[len - 1]))
		{
			ly.SemanticIndex = atoi(&smName[len - 1]);
			StringUtil::DelChar(smName, smName[len - 1]);
		}
		else
		{
			ly.SemanticIndex = 0;
		}
		
	
		ly.SemanticName = move(smName);
		// �Է� ���̾ƿ� ����� ����..
		m_vLayout.push_back(ly);
	}
	
}
bool LayoutInformation::Decryptable(const std::string& sentence)
{
	if (StringUtil::FindString(sentence, hlslType::INPUTLAYOUT.c_str()) || IsProgressing())
	{
		return true;
	}
	return false;
}
void LayoutInformation::WriteShaderData(std::ofstream& fout)
{
	fout << "@@ InputLayout" << endl;
	fout << "Count : " << m_vLayout.size() << endl;
	for (auto& iter : m_vLayout)
	{
		fout << iter.SemanticName << " " << iter.SemanticIndex << " " << iter.Format << " "
			 << iter.InputSlot << " " << iter.AlignedByteOffset << " " << iter.InputSlotClass << " "
			 << iter.InstanceDataStepRate << endl;
	}
	fout << "@@" << endl;
}