#pragma once
#include"Common/JGRCCommon.h"
#include"DirectXCommon.h"

namespace JGRC
{
	/*
	Exp : 샘플러 모드, Wrap, Clamp, 커스텀 모드가 있다. */
	enum class CORE_EXPORT ESamplerMode
	{
		Default_Wrap,
		Default_Clamp,
		Custom
	};
	class CORE_EXPORT InputLayoutData
	{
	public:
		std::vector<std::string> vSementicNames;
		std::vector<D3D11_INPUT_ELEMENT_DESC> vDescs;
		std::vector<uint> vStrides;
	public:
		D3D11_INPUT_ELEMENT_DESC* Get();
	};

	class CORE_EXPORT CBufferData
	{
	public:
		class Var
		{
		public:
			std::vector<real> Value;
			std::string Name;
		public:
			void varInit(const uint count)
			{
				for (uint i = 0; i < count; ++i)
				{
					Value.push_back(0);
				}
			}
		};
	public:
		std::string Name;
		uint        Size;
		std::vector<Var> vVars;
	public:
		void* getData();
		real* getValue(const std::string& name);
		bool  setValue(const std::string& name, real* data);
		uint  getCount(const std::string& name);
	private:
		std::vector<real> Data;
	};
	class CORE_EXPORT SamplerStateData
	{
	public:
		std::vector<D3D11_SAMPLER_DESC> vDescs;
		std::vector<ESamplerMode>       vModes;
	public:
		D3D11_SAMPLER_DESC* GetDescs(const uint idx);
	};

	class CORE_EXPORT TextureData
	{
	public:
		std::vector<std::string> vNames;
		std::vector<class JGTexture*> vTexture;
		// 텍스쳐도 추가.
	};

}
