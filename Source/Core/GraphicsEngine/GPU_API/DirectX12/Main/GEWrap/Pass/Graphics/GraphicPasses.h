#pragma once
#include "GraphicPassDefine.h"


namespace DX12
{
	class Entry : public DX12_GraphicPass
	{
	public:
		static std::string ShaderPath;

	public:
		Entry();
	};



	class GBuffer : public DX12_GraphicPass
	{
	public:
		static std::string ShaderPath;

	public:
		GBuffer();
	};


	class Lighting : public DX12_GraphicPass
	{
	public:
		static std::string ShaderPath;

	public:
		Lighting();
	};



	class ShadowMap : public DX12_GraphicPass
	{
	public:
		static std::string ShaderPath;

	public:
		ShadowMap();
	};
	/*
	���� ����   ����  ���� �� �� 
	����Ʈ ���� ����
	����Ʈ ��� worldviewproj
	*/

}