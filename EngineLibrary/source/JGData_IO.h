#pragma once
#include"Common.h"

namespace JGLibrary
{
	typedef unsigned long long jg_offset;
	typedef unsigned long long jg_size;
	typedef unsigned long long jg_datapos;
	typedef unsigned int jg_ID;

	/*
	Exp : ������ ����� ��� */
	enum class EData_IO_Mode
	{
		Write, // ����
		Read   // �б�
	};
	/*
	Exp : ������ ��� ���� */
	typedef struct Data_Header
	{
		jg_offset offset = 0; // ��ġ
		jg_size   size   = 0; // ������
	}Data_Header;

	class JGData_IO
	{
	private:
		EData_IO_Mode  m_ioMode;  // I/O  ���
		std::ofstream  m_fout;    
		std::ifstream  m_fin;
	private:
		//std::vector<jg_ID>       m_ID;
		//std::unordered_map<jg_ID, Data_Header> m_umHeaders;
		std::vector<Data_Header> m_headerData; // ������ ��� ���� �迭
		std::vector<byte> m_data;              // �������� ������
		jg_datapos        m_curr_off    = 0;               // ���� ������
		jg_size           m_fileSize    = sizeof(jg_size); // ���� ������
		jg_size           m_headerCount = 0;              // ��� ����
		jg_size           m_dataSize    = 0;               // ������ ������
	public:
		JGData_IO(const EData_IO_Mode IOmode);
		JGData_IO(const JGData_IO& io);
		JGData_IO(JGData_IO&& io);
	public:
		JGData_IO& operator=(const JGData_IO& io);
		JGData_IO& operator=(JGData_IO&& io);
	public:
		/*
		@param data     : Ǫ���� ������ 
		@param dataSize : Ǫ���� ������ ������ */
		bool DataPush(void* data, const jg_size dataSize);
		bool Write(const std::string& path);
		bool Read(const std::string& path);
	public:
		const jg_size GetFileSize() const;
		const jg_size GetDataSize() const;
		const byte*   GetData() const;
	public:
		void DataInfo_printcpp() const;
	};
}
