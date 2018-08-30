#pragma once
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<vector>
#include<experimental/filesystem>



/*
Class : JGClassTypeManger 
@param path m_OutputPath : Ŭ���� Ÿ�������� ����ִ� ������ ������ ���
@param vector<string> m_vConstructClassType : Ŭ���� ������ ����ִ� ������ �ҷ��� string���·� ������ ���� 
# - Dubug����϶��� ���� 
@param vector<path> m_vClassHeaderFile : ������ϸ� ��� ����
@param vector<std::string> m_vClassTypeToSave : ������ Ŭ���� Ÿ�� �迭
@param path m_EntryPath : �˻��� ���� ���� */
class JGClassTypeManager
{
private:
	std::experimental::filesystem::path m_OutputPath;
	std::vector<std::string> m_vConstructClassType;
#ifdef _DEBUG
	//Class type to store
	std::vector<std::experimental::filesystem::path> m_vClassHeaderFile;
	std::vector<std::string> m_vClassTypeToSave;
	std::experimental::filesystem::path m_EntryPath;
#endif
public:
	/*
	Exp : ��� ������ �Ѵ�. 
	@param const wstring EntryPath : Ž�� �������� ��� 
	@param const wstring OutputPath :  ���� ������ ��� */
	void SettingEntryPath(const std::wstring& EntryPath, const std::wstring& OutputPath);
	/*
	Exp : Ÿ���� �����ϰ� ���� ���·� �ҷ��帰��. */
	bool SearchAllClassTypeOfDirectory();
private:
	bool LoadClassType();
	bool RegisterClassType(const std::string& FileName);
	bool WriteRegisteredClassType();
	bool FindHeaderFile(const std::experimental::filesystem::path& CurrentPath);
	bool IsHeaderFile(std::wstring& Path);
};