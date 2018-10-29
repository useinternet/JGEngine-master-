#include"LayoutInformation.h"
using namespace std;
using namespace JGRC;

const string LayoutInformation::INPUTLAYOUT = "INPUTLAYOUT";
const string LayoutInformation::INPUTSLOT   = "INPUTSLOT";
const string LayoutInformation::INSTNACE    = "INSTANCE";
const string LayoutInformation::FLOAT  = "float";
const string LayoutInformation::FLOAT2 = "float2";
const string LayoutInformation::FLOAT3 = "float3";
const string LayoutInformation::FLOAT4 = "float4";
LayoutInformation::LayoutInformation()
{

}
LayoutInformation::~LayoutInformation()
{

}
bool LayoutInformation::IsProgressing() const
{
	return m_bIsProgressing;
}
void LayoutInformation::AnalyzeSentence(string& sentence)
{
	if (StringUtil::FindString(sentence, "#define"))
	{
		return;
	}
	m_bIsProgressing = true;
	// �ΰ����� ������ �ǳʶٱ�
	if (StringUtil::FindString(sentence, "struct") || sentence == "{" || sentence == "}")
	{
		if (sentence == "}")
			m_bIsProgressing = false;
		return;
	}
	// �Է� ���� �߰�
	else if (StringUtil::FindString(sentence,INPUTSLOT.c_str()))
	{
		inputSlot = ExtractionBracketNumber(sentence);
	}
	// �ν��Ͻ� ���� �Է� 
	else if (StringUtil::FindString(sentence, INSTNACE.c_str()))
	{
		m_bIsInstance = true;

	}
	else if (StringUtil::FindString(sentence,":"))
	{

		// ������ ���� �ޱ�
		if (StringUtil::FindString(sentence, FLOAT.c_str()))
		{

		}
		else if (StringUtil::FindString(sentence, FLOAT2.c_str()))
		{

		}
		else if (StringUtil::FindString(sentence, FLOAT3.c_str()))
		{

		}
		else if (StringUtil::FindString(sentence, FLOAT4.c_str()))
		{

		}
		// �ø�ƽ �̸� ���
		uint len = sentence.length();
		uint pos = sentence.find(':');
		string smName = sentence.substr(pos + 1, len - 1);
		StringUtil::DelChar(';');
		StringUtil::DelBlank(smName);
	}
}
uint LayoutInformation::ExtractionBracketNumber(const std::string& s)
{
	bool StartPos = false;
	string number = "";
	for (auto iter = s.begin(); iter < s.end(); ++iter)
	{
		if (*iter == ')')
			break;
		if (StartPos)
		{
			number += *iter;
		}
		if (*iter == '(')
			StartPos = true;
	}
	StringUtil::DelBlank(number);

	return atoi(number.c_str());
}