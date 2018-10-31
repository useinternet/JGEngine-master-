#include"ShaderAnalyzer.h"
#include"LayoutInformation.h"
#include"CBufferInformation.h"
#include"SamplerInformation.h"
#include"TextureInformation.h"
#include"MaterialSystem/Shader/ShaderTool/InputLayout.h"
#include"BufferSystem/JGBufferManager.h"
#include"BufferSystem/JGBuffer.h"
#include"DirectX/DirectX.h"
using namespace JGRC;
using namespace std;




ShaderAnalyzer::ShaderAnalyzer()
{
	m_BufferMG = JGBufferManager::GetInstance();
	m_DirectX  = DirectX::GetInstance();
	m_InputLayoutInfor = make_unique<LayoutInformation>();
	m_CBufferInfor     = make_unique<CBufferInformation>();
	m_SamplerInfor     = make_unique<SamplerInformation>();
	m_TextureInfor     = make_unique<TextureInformation>();
}
ShaderAnalyzer::~ShaderAnalyzer()
{
	for (auto& iter : m_vBuffers)
	{
		m_BufferMG->DeleteBuffer(iter);
	}
}
bool ShaderAnalyzer::Analyze(const string& hlslPath, const EShaderType ShaderType)
{
	m_ShaderType = ShaderType;
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
		else if (m_InputLayoutInfor->Decryptable(buffer))
		{
			m_InputLayoutInfor->AnalyzeSentence(buffer);
		}
		// ��� ���� ����
		else if (m_CBufferInfor->Decryptable(buffer))
		{
			m_CBufferInfor->AnalyzeSentence(buffer);
		}
		else if (m_SamplerInfor->Decryptable(buffer))
		{
			m_SamplerInfor->AnalyzeSentence(buffer);
		}
		else if (m_TextureInfor->Decryptable(buffer))
		{
			m_TextureInfor->AnalyzeSentence(buffer);
		}
	}
	fin.close();
	return true;
}
void ShaderAnalyzer::CreateConstantBuffers()
{
	for (uint i = 0; i < m_CBufferInfor->GetCBufferCount(); ++i)
	{
		CBuffer* cbf = m_CBufferInfor->GetCBuffer(i);
		JGBuffer* buffer = m_BufferMG->CreateBuffer(
			EBufferType::ConstantBuffer, EUsageType::Dynamic, ECPUType::Access_Write,
			nullptr, cbf->size(), 1);
		m_vBuffers.push_back(buffer);
	}
}
void ShaderAnalyzer::WriteConstantBuffers()
{
	for (uint i = 0; i < m_CBufferInfor->GetCBufferCount(); ++i)
	{
		CBuffer* cbf = m_CBufferInfor->GetCBuffer(i);
		std::vector<real> data;
		cbf->getData(&data);
		m_vBuffers[i]->Write(EMapType::Write_Discard, &data[0]);
		switch (m_ShaderType)
		{
		case EShaderType::Pixel:
			m_DirectX->GetContext()->PSSetConstantBuffers((UINT)i, 1, m_vBuffers[i]->GetAddress());
			break;
		case EShaderType::Vertex:
			m_DirectX->GetContext()->VSSetConstantBuffers((UINT)i, 1, m_vBuffers[i]->GetAddress());
			break;
		}
	}
}
void ShaderAnalyzer::MakeInputLayoutArray(class InputLayout* ly)
{
	m_InputLayoutInfor->MakeInputLayoutArray(ly);
}
float*   ShaderAnalyzer::GetParam(const std::string& paramName)
{
	return m_CBufferInfor->GetParam(paramName);
}
void     ShaderAnalyzer::SetParam(const std::string& paramName, void* value)
{
	m_CBufferInfor->SetParam(paramName, value);
}
uint     ShaderAnalyzer::GetParamSize(const std::string& paramName)
{
	return m_CBufferInfor->GetParamSize(paramName);
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