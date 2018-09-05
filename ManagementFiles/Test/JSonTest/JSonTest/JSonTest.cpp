// JSonTest.cpp: 콘솔 응용 프로그램의 진입점을 정의합니다.
//

#include "stdafx.h"
#include<iostream>
#include<fstream>
#pragma warning(disable : 4996)
#include"../../../Include/Json/json/json.h"

int main()
{
	Json::Value root;
	root["level"] = 123;
	root["id"] = "qwerty";
	root["password"] = "qwerty";
	root["isBeta"] = false;

	Json::Value item;
	item.append("stone");
	item.append("Dirt");
	root["item"] = item;

	Json::Value friendList;
	Json::Value billy;
	billy["level"] = 1;
	Json::Value sam;
	sam["level"] = 10;
	friendList.append(billy);
	friendList.append(sam);
	root["friendList"] = friendList;

	Json::StyledWriter writer;
	std::string str = writer.write(root);

	std::ofstream stream;
	stream.open("Test.json");
	stream << str;


    return 0;
}

