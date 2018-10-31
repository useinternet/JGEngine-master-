#include"StringUtil.h"
#include<cstring>
#include<assert.h>
using namespace std;
bool StringUtil::FindString(const string& sentence, const char* str)
{
	uint count = 0;
	uint strLength = strlen(str);
	for (uint i = 0; i < sentence.length(); ++i)
	{
		if (sentence[i] == str[count])
		{
			count++;
		}
		else
		{
			count = 0;
		}
		if (count == strLength)
		{
			return true;
		}
	}
	return false;
}
void StringUtil::DelString(string& str, const char* delstr)
{
	bool bSearching = false;    // ���� Ž��������..
	uint len = str.length();
	uint dellen = strlen(delstr);
	uint count = 0;
	string result;                  // Ž���Ҷ� �ӽ� ���ڿ�
	string tmpStr;


	for (uint i = 0; i < len; ++i)
	{
		// ���ڰ� ������ Ž�� ����.
		if (str[i] == delstr[count])
		{
			bSearching = true;
			tmpStr += str[i];
			count++;
		}
		// �ƴϸ� �ʱ�ȭ
		else
		{
			if (bSearching && count < dellen)
			{
				result += tmpStr;

			}
			result += str[i];
			tmpStr = "";
			bSearching = false;
			count = 0;
		}
		// Ž�� ���̾��� ���� ���ڰ� �ƴϾ����� �ӽ÷� �����ϰ��־��� ���ڿ��� �����δ�.
		// �װ� �ƴ϶�� �׳� �߰� ���� �ʴ´�.

	}
	str = result;
}
void StringUtil::DelChar(std::string& str, const char c)
{
	for (auto iter = str.begin(); iter < str.end();)
	{
		if (*iter == c)
		{
			iter = str.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
void StringUtil::DelBlank(string& str)
{
	DelChar(str, ' ');
}
void StringUtil::Path_DelEndPath(std::string& str)
{
	string result;
	uint count = 0;  // ������ �ܾ��� ��
	uint delPos = 0; // ������ ������ �ܾ��� ��ġ
    // ������ ��ġ�� Ž��
	for (auto iter = str.end() - 1; iter >= str.begin(); --iter)
	{
		if (*iter == '/')
		{
			break;
		}
		count++;
	}
	// ����
	delPos = str.length() - count;
	for (auto iter = str.begin() + delPos; iter < str.end();)
	{
		iter = str.erase(iter);
	}
}