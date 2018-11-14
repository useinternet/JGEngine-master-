#include"ShaderInformation.h"
#include<cstring>
using namespace std;
using namespace JGRC;





uint ShaderInformation::ExtractionBracketNumber(const std::string& s)
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
float ShaderInformation::ExtractionBracketFloat(const string& s)
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
	
	return (float)atof(number.c_str());
}
void ShaderInformation::ExtractionBracketComaFloat(const string& s, float* outPut)
{
	if (outPut == nullptr)
	{
		return;
	}
	bool StartPos = false;
	uint floatCount = 0;
	string number = "";
	for (auto iter = s.begin(); iter < s.end(); ++iter)
	{
		// ���� ��ȣ�̸� �극��ũ
		if (*iter == ')')
			break;
		// ��ȣ�� ������ ��������
		if (StartPos)
		{
			// ���� ����
			if (*iter == ',')
			{
				StringUtil::DelBlank(number);
				outPut[floatCount++] = (float)atof(number.c_str());
				number = "";
			}
			else
			{
				number += *iter;
			}
		}
		// ��ȣ ���Ǵٰ� �˸�
		if (*iter == '(')
			StartPos = true;
	}
	StringUtil::DelBlank(number);
	outPut[floatCount++] = (float)atof(number.c_str());
}
void ShaderInformation::ExtractionBracketComaNumber(const string& s, uint* outPut)
{
	if (outPut == nullptr)
	{
		return;
	}
	bool StartPos = false;
	uint uintCount = 0;
	string number = "";
	for (auto iter = s.begin(); iter < s.end(); ++iter)
	{
		// ���� ��ȣ�̸� �극��ũ
		if (*iter == ')')
			break;
		// ��ȣ�� ������ ��������
		if (StartPos)
		{
			// ���� ����
			if (*iter == ',')
			{
				StringUtil::DelBlank(number);
				outPut[uintCount++] = atoi(number.c_str());
				number = "";
			}
			else
			{
				number += *iter;
			}
		}
		// ��ȣ ���Ǵٰ� �˸�
		if (*iter == '(')
			StartPos = true;
	}
	StringUtil::DelBlank(number);
	outPut[uintCount++] = atoi(number.c_str());
}