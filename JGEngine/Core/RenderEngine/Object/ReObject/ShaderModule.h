#pragma once




#include "Object/ReObject.h"

namespace RE
{
	/*
	-- ShaderModule, GlobalShaderDataArea, GlobalShaderFuncArea, ShaderDataAream,ShaderFuncArea 
	1. ShaderModule �ϳ��� �ҽ��ڵ� �ϳ�
	2. Shader ���(�׷���) -> �ڵ� ����
	  - inputlayout
	  - constant buffer
	  - structed buffer
	  - texture
	  - unordered texture
	  - var
	  - const, static 
	  - sampler
	  - 
	3. ������ �ܰ躰


     ShaderData
	 int
	 str



	*/
	class ShaderModule : public ReObject
	{
	public:
		struct InputLayout
		{
			
			ShaderData data;
			DXGI_FORMAT format;
		};
		struct ConstantBuffer
		{
			// ���ε� ��ȣ
			// Data
			//
		};
	public:
		

	};


	class GraphicsShaderModule : public ShaderModule
	{


	};
	class ComputeShaderModule : public ShaderModule
	{




	};

}

