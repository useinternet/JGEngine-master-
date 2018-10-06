// JGLogEngine.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//
#include<Windows.h>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;
// Error : 252
// default : 240
// warning : 014
// process : 242
int ColorChange(const string& log)
{
	if (log == "1001")
	{
		return 252;
	}
	if (log == "0001")
	{
		return 112;
	}
	if (log == "1101")
	{
		return 014;
	}
	if (log == "0101")
	{
		return 242;
	}
	return 0;
}
int main()
{
	HANDLE hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ifstream file;
	string log;
	file.open("JGLogOuput.txt");
	while (true)
	{
		while (getline(file, log))
		{
			int color = ColorChange(log);
			if (color == 0)
			{
				cout << log << endl;
			}
			else
			{
				SetConsoleTextAttribute(hConsole, color);
			}
		}
		if (!file.eof()) break;
		file.clear();
	}
	file.close();
    return 0;
}

