#include "pch.h"
#include "ShaderLib.h"
#include "ShaderDataType.h"

using namespace std;
namespace RE
{
	STStruct* ShaderTypeLib::AddStruct(const std::string& struct_name)
	{
		if (m_ShaderStructPool.find(struct_name) != m_ShaderStructPool.end())
		{
			return nullptr;
		}
		std::shared_ptr<STStruct> st = make_shared<STStruct>(struct_name);
		STStruct* result = st.get();
		st->m_SortingOrder = m_SortingIndex++;
		m_ShaderStructPool[struct_name] = st;
		m_SortingOrderMap[st->m_SortingOrder] = result;

		return result;
	}

	STStruct* ShaderTypeLib::FindStruct(const std::string& struct_name)
	{
		if (m_ShaderStructPool.find(struct_name) == m_ShaderStructPool.end())
		{
			return nullptr;
		}
		return m_ShaderStructPool[struct_name].get();
	}

	std::shared_ptr<STStruct>  ShaderTypeLib::CloneStruct(const std::string& struct_name)
	{
		auto st = FindStruct(struct_name);
		if (st)
		{
			return make_shared<STStruct>(*st);
		}
		return nullptr;
	}
	void ShaderTypeLib::SetSortingOrder(const std::string& struct_name, uint32_t sorting_order)
	{
		if (m_ShaderStructPool.find(struct_name) == m_ShaderStructPool.end())
		{
			return;
		}
		if (m_SortingOrderMap.find(sorting_order) == m_SortingOrderMap.end())
		{
			return;
		}

		auto v1 = m_ShaderStructPool[struct_name].get();
		auto v2 = m_SortingOrderMap[sorting_order];

		auto s1 = v1->m_SortingOrder;
		auto s2 = v2->m_SortingOrder;

		m_SortingOrderMap[s2] = v1;
		m_SortingOrderMap[s1] = v2;
	}
	void ShaderTypeLib::RemoveStruct(const std::string& struct_name)
	{
		if (m_ShaderStructPool.find(struct_name) == m_ShaderStructPool.end())
		{
			return;
		}

		uint32_t sorting_order = m_ShaderStructPool[struct_name]->m_SortingOrder;
		

		for (uint32_t i = sorting_order + 1; i < m_SortingIndex; ++i)
		{
			auto st = m_SortingOrderMap[i];

			m_SortingOrderMap.erase(i);
			m_SortingOrderMap[i - 1] = st;
			st->m_SortingOrder = i - 1;
		}
		--m_SortingIndex;

		m_ShaderStructPool.erase(struct_name);
	}
	bool ShaderTypeLib::Load(const std::string& path)
	{
		/*
1. Group, Format,  ����ü ����
2. Struct ����
   - Struct �̸� �� ��� ����
   - Struct Var(Type, Type�̸�, Var�̸�)
3. �ӽ��ڵ�
4. ������ �ڵ�
*/
		std::ifstream fin(path, ios::binary);
		if (!fin.is_open())
		{
			return false;
		}
		EAssetGroup   group;
		EAssetFormat  format;
		size_t struct_count = 0;
		// 1�� 
		DataIO::read(fin, group);
		DataIO::read(fin, format);
		DataIO::read(fin, struct_count);
		//fin >> group >> format >> struct_count;

		for (size_t i = 0; i < struct_count; ++i)
		{
			// 2�� (�̸��� ��� ����)
			std::string struct_name;
			uint32_t element_count = 0;


			DataIO::read(fin, struct_name);
			DataIO::read(fin, element_count);

			//fin >> struct_name >> element_count;

			std::shared_ptr<STStruct> st_struct = make_shared<STStruct>(struct_name);
			// 2�� ����ü ����( Type, type�̸�, var�̸�)
			for (uint32_t j = 0; j < element_count; ++j)
			{
				JGShader::EShaderData type;
				std::string type_name;
				std::string var_name;

				DataIO::read(fin, type);
				DataIO::read(fin, type_name);
				DataIO::read(fin, var_name);


				//fin >> type >> type_name >> var_name;


				if (type == JGShader::_struct)
				{
					if (m_ShaderStructPool.find(type_name) == m_ShaderStructPool.end())
					{
						STStruct st(type_name);
						st_struct->Add(st, var_name);
						m_IsSuccess = false;
					}
					else
					{
						st_struct->Add(*m_ShaderStructPool[type_name], var_name);
					}
		
				}
				else
				{
					st_struct->Add(type, var_name);
				}
			}
			m_ShaderStructPool[st_struct->GetName()] = st_struct;
			m_SortingOrderMap[i] = st_struct.get();
		}
		m_SortingIndex = (uint32_t)m_ShaderStructPool.size();
		if (m_IsSuccess)
		{
			m_SuccessCode = GetCode();
		}
		return true;
	}
	bool ShaderTypeLib::Save(const std::string& path)
	{
		/*
1. Group, Format, ����ü ����
   
2. Struct ����
   - Struct �̸� �� ��� ����
   - Struct Var(Type, Type�̸�, Var�̸�)
3. �ӽ��ڵ�
4. ������ �ڵ�
*/
		std::ofstream fout;
		fout.open(path, ios::binary);
		if (!fout.is_open())
		{
			return false;
		}
		// 1��
		// �׷�� ���� ����  ����ü ��������
		EAssetGroup  group  = EAssetGroup::AssetGroup_RE;
		EAssetFormat  format = EAssetFormat::AssetFormat_RE_ShaderTypeLib;
		DataIO::write(fout, group);
		DataIO::write(fout, format);
		DataIO::write(fout, m_ShaderStructPool.size());
		//fout << group << format << m_ShaderStructPool.size();


		for (auto& struct_pair : m_SortingOrderMap)
		{
			auto ststruct = struct_pair.second;


			// 2�� 
			// ����ü �̸��� ��� ���� ����
			DataIO::write(fout, ststruct->GetName());
			DataIO::write(fout, ststruct->GetElementCount());
			//fout << ststruct->GetName() << ststruct->GetElementCount();

			// �� ����ü ��� ����
			for (uint32_t i = 0; i < ststruct->GetElementCount(); ++i)
			{
				auto element = ststruct->GetElement(i);


				DataIO::write(fout, element->GetType());
				DataIO::write(fout, element->GetName());
				DataIO::write(fout, ststruct->GetElementName(i));


				//fout << (uint32_t)element->GetType() << element->GetName() << ststruct->GetElementName(i);
			}

		}

		DataIO::write(fout, GetCode());
		DataIO::write(fout, m_SuccessCode);
		//fout << GetCode();
		//fout << m_SuccessCode;
		fout.close();
		return true;
	}
	std::string ShaderTypeLib::GetCode() const
	{

		std::string code =
			"#ifndef " + GetName() + "SHADERLIB \n" +
			"#define " + GetName() + "SHADERLIB \n";

		for (auto& st_struct_pair : m_SortingOrderMap)
		{

			auto ststruct = st_struct_pair.second;
			code += ststruct->GetShaderDefineCode();
		}

		code += "#endif";
		return code;
	}

	std::string ShaderTypeLib::GetSuccessedCode() const
	{
		if (m_SuccessCode.empty())
		{
			return GetCode();
		}
		return m_SuccessCode;
	}

	ShaderLibManager::ShaderLibManager() : ReObject("ShaderLibManager")
	{
		m_ShaderTypeLib = make_shared<ShaderTypeLib>();
		m_ShaderTypeLib->SetName("ShaderTypeLib");
		ReObject::m_ShaderLibManager = this;
	//	m_ShaderFuncLib = make_shared<ShaderFunctionLib>("ShaderFunctionLib");
	}


	void ShaderLibManager::Load(const std::string& path)
	{
		m_ShaderTypeLib->Load(path);
	}
	void ShaderLibManager::Save(const std::string& path)
	{
		m_ShaderTypeLib->Save(path);
	}

	ShaderTypeLib* ShaderLibManager::GetTypeLib()
	{
		return m_ShaderTypeLib.get();
	}
	std::string ShaderLibManager::GetCode() const
	{
		std::string code;
		code += m_ShaderTypeLib->GetCode();
		return code;
	}
}