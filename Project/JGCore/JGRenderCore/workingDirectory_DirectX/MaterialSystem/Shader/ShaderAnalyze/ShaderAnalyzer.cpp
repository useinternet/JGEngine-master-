#include"ShaderAnalyzer.h"
#include"LayoutInformation.h"
#include"CBufferInformation.h"
#include"SamplerInformation.h"
#include"TextureInformation.h"
using namespace JGRC;
using namespace std;




ShaderAnalyzer::ShaderAnalyzer()
{

	m_InputLayoutInfor = make_unique<LayoutInformation>();
	m_CBufferInfor     = make_unique<CBufferInformation>();
	m_SamplerInfor     = make_unique<SamplerInformation>();
	m_TextureInfor     = make_unique<TextureInformation>();
}
ShaderAnalyzer::~ShaderAnalyzer()
{


}
bool ShaderAnalyzer::Analyze(const string& hlslPath, const EShaderType ShaderType)
{
	fstream fin;
	string buffer; // �Է� �޾ƿ� ���ڿ�
	string path;   // �о�

	fin.open(hlslPath.c_str());
	if (!fin.is_open())
	{
		return false;
	}
	while (!fin.eof())
	{
		getline(fin, buffer);
		if (!RemoveRemark(buffer))
		{
			continue;
		}

		// ��ũ����� ��� hlsl������ ��� Ž���Ѵ�.
		if (StringUtil::FindString(buffer, hlslType::INCLUDE.c_str() ))
		{
			path = IncludeAyz(hlslPath,buffer);
			Analyze(path, ShaderType);
		}
		// �Է� ���̾ƿ� ���� ����
		else if (StringUtil::FindString(buffer, hlslType::INPUTLAYOUT.c_str()) || m_InputLayoutInfor->IsProgressing())
		{
			m_InputLayoutInfor->AnalyzeSentence(buffer);
		}
		// ��� ���� ����
		else if (StringUtil::FindString(buffer, hlslType::CBUFFER.c_str()) || m_CBufferInfor->IsProgressing())
		{
			m_CBufferInfor->AnalyzeSentence(buffer);
		}
		else if (StringUtil::FindString(buffer, hlslType::SAMPLER_Start.c_str()) || m_SamplerInfor->IsProgressing())
		{
			m_SamplerInfor->AnalyzeSentence(buffer);
		}
		else if (StringUtil::FindString(buffer, hlslType::TEXTURE_Start.c_str()) || m_TextureInfor->IsProgressing())
		{
			m_TextureInfor->AnalyzeSentence(buffer);
		}
	}
	fin.close();
	return true;
}
bool ShaderAnalyzer::RemoveRemark(std::string& sentence)
{
	if (StringUtil::FindString(sentence, "//"))
	{
		uint pos = sentence.find("//");
		if (pos < 1)
		{
			return false;
		}
		StringUtil::DelString(sentence, sentence.substr(pos, sentence.length() - 1).c_str());
	}
	if (StringUtil::FindString(sentence, "*/"))
	{
		uint pos = sentence.find("*/");
		m_bIgnore = false;
		if (pos < 1)
		{
			return false;
		}
		StringUtil::DelString(sentence, sentence.substr(0, pos + 2).c_str());
		return true;
	}
	else if (StringUtil::FindString(sentence, "/*"))
	{
		uint pos = sentence.find("/*");
		m_bIgnore = true;
		if (pos < 1)
		{
			return false;
		}
		StringUtil::DelString(sentence, sentence.substr(pos, sentence.length() - 1).c_str());
		return true;
	}
	if (m_bIgnore)
	{
		return false;
	}
	return true;
}
string ShaderAnalyzer::IncludeAyz(string hlslPath, string& sentence)
{
	// ��ũ����� path�� ���� �����´�.
	StringUtil::DelString(sentence, "#include");
	StringUtil::DelChar(sentence, '\"');

	StringUtil::Path_DelEndPath(hlslPath);
	return hlslPath + sentence;
}