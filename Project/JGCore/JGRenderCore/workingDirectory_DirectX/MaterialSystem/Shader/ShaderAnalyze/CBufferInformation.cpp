#include"CBufferInformation.h"

using namespace JGRC;
using namespace std;
uint CBuffer::size() const
{
	uint result = 0;
	for (auto& iter : mVars)
	{
		result += iter.second.VarSize;
	}
	return result;
}
void  CBuffer::getData(std::vector<float>* dataArray)
{
	for (auto& iter : mVars)
	{
		for (auto& var : iter.second.vValue)
		{
			dataArray->push_back(var);
		}
	}
}
CBufferInformation::CBufferInformation()
{

}
CBufferInformation::~CBufferInformation()
{

}

void CBufferInformation::AnalyzeSentence(std::string& sentence)
{
	Start();
	if (StringUtil::FindString(sentence, "struct") || sentence == "{" || sentence == "};" || sentence == "}")
	{
		if (sentence == "};" || sentence == "}")
		{
			m_mCBuffers.insert(pair<uint, CBuffer>(m_bufferNum, m_buffer));
			Stop();
			CBufferClear();
		}
		return;
	}
	if (StringUtil::FindString(sentence, "cbuffer"))
	{
		StringUtil::DelString(sentence, "cbuffer");
		StringUtil::DelBlank(sentence);
		m_buffer.CBufferName = sentence;
	}
	// ������ �׳� ��������.
	else if(!BlankCheck(sentence) && sentence != "")
	{
		CBufferVar var;
		var.VarName = sentence;
		// ������ �˾Ƴ���.( ���� ������� �� )
		if (StringUtil::FindString(sentence, hlslType::MATRIX.c_str()))
		{
			var.VarSize = sizeof(float) * 16;
			StringUtil::DelString(var.VarName, hlslType::MATRIX.c_str());
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT4.c_str()))
		{
			var.VarSize = sizeof(float) * 4;
			StringUtil::DelString(var.VarName, hlslType::FLOAT4.c_str());
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT3.c_str()))
		{
			var.VarSize = sizeof(float) * 3;
			StringUtil::DelString(var.VarName, hlslType::FLOAT3.c_str());
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT2.c_str()))
		{
			var.VarSize = sizeof(float) * 2;
			StringUtil::DelString(var.VarName, hlslType::FLOAT2.c_str());
		}
		else if (StringUtil::FindString(sentence, hlslType::FLOAT.c_str()))
		{
			var.VarSize = sizeof(float);
			StringUtil::DelString(var.VarName, hlslType::FLOAT.c_str());
		}
		for (uint i = 0; i < (var.VarSize / sizeof(float)); ++i)
		{
			var.vValue.push_back(0.0f);
		}
		// ���� �̸� ����
		StringUtil::DelChar(var.VarName, '\t');
		StringUtil::DelString(var.VarName, ";");
		StringUtil::DelBlank(var.VarName);

		// ���� ������ۿ� ����.
		m_buffer.mVars.insert(pair<uint,CBufferVar>(m_varNum++, var));
	}
}
bool CBufferInformation::Decryptable(const std::string& sentence)
{
	if (StringUtil::FindString(sentence, hlslType::CBUFFER.c_str()) || IsProgressing())
	{
		return true;
	}
	return false;
}
CBuffer* CBufferInformation::GetCBuffer(const uint idx)
{
	return &m_mCBuffers[idx];
}
float*  CBufferInformation::GetParam(const std::string& paramName) 
{
	return &FindVar(paramName)->vValue[0];
}
void   CBufferInformation::SetParam(const std::string& paramName, void* value)
{
	// ptr ���� ó��
	CBufferVar* ptr = FindVar(paramName);
	memcpy(&ptr->vValue[0], value, ptr->VarSize);
}
uint   CBufferInformation::GetParamSize(const std::string& paramName)
{
	return FindVar(paramName)->VarSize;
}
uint   CBufferInformation::GetCBufferCount()
{
	return m_mCBuffers.size();
}
bool CBufferInformation::BlankCheck(const std::string& sentence)
{
	for (uint i = 0; i < sentence.length(); ++i)
	{
		if (sentence[i] != ' ')
		{
			return false;
		}
	}
	return true;
}
void CBufferInformation::CBufferClear()
{
	m_buffer.CBufferName = "";
	m_buffer.mVars.clear();
	m_bufferNum++;
	m_varNum = 0;
}
CBufferVar* CBufferInformation::FindVar(const std::string& name)
{
	for (auto& iter : m_mCBuffers)
	{
		for (auto& var : iter.second.mVars)
		{
			if (var.second.VarName == name)
			{
				return &var.second;
			}
		}
	}
	return nullptr;
}

