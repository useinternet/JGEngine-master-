#pragma once
#include "Object/ReObject.h"

namespace RE
{
	class GraphicsShaderModule;
	class GraphicsPipelineState;
	class Texture;
	class Shader;
	enum class EReMaterialUsage
	{
		GUI,
		Static3D,
		Skeletal3D
	};
	/* ���͸����� ���
	- ���̴� ���� ����
	- Pixel �ڵ� ����
	- �ؽ��� ����
	- �� ���͸��� �뵵�� �°� ž��� ���̴� ���� ������.
	- ������ �ϱ��� �ؽ��� �̸��� �ٲ۴�.
	**  ���� ������ �� **
	- �ؽ��� ����
	*/
	class ReMaterial : public ReObject
	{
		static std::map<EReMaterialUsage, std::string> sm_ModuleNameByUsage;
	public:
		GraphicsPipelineState* GetPSO() const {
			return m_PSO.get();
		}
		ReMaterial();
	public:
		void SetBlendState(const D3D12_BLEND_DESC& desc);
		void SetDepthStencilState(const D3D12_DEPTH_STENCIL_DESC& desc);
		void SetRasterizerState(const D3D12_RASTERIZER_DESC& desc);
		bool AddTexture(const std::string& name, const std::string& asset_texture_path);
		void SetCode(const std::string& code);
		bool Compile();

		const Texture& GetBindedTexture(uint32_t idx) const;
		const Texture& GetBindedTexture(const std::string& name);
		uint32_t GetBindedTextureCount() const;
		const std::vector<Texture>& GetBindedTextures() const;
	public:
		static void BindShaderModuleByUsage(EReMaterialUsage usage, const std::string& module_name);
	private:
		std::shared_ptr<GraphicsPipelineState>        m_PSO;
		EReMaterialUsage m_Usage = EReMaterialUsage::Static3D;
		std::unordered_map<std::string, uint32_t>     m_TextureIndexMap;
		std::vector<Texture>                          m_BindedTextures;
		std::string m_PixelCode;
		std::shared_ptr<Shader> m_CompiledPixelShader;
	};


	
}