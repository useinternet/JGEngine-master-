#pragma once
#include"DxCommon/DxCommon.h"
namespace JGRC
{
	
	/*
	Exp : ShaderResourceView Pack 
	* ���ҽ�
	* ���ҽ� ��� */
	struct SrvResourcePack
	{
		std::string PackName;
		ID3D12Resource* Resource = nullptr;
		D3D12_SHADER_RESOURCE_VIEW_DESC Desc;
	};
	/*
	Exp : UnorderedAccessView Pack 
	* ���ҽ�
	* ���ҽ� ��� */
	struct UavResourcePack
	{
		std::string PackName;
		ID3D12Resource* Resource = nullptr;
		ID3D12Resource* CounterResource = nullptr;
		D3D12_UNORDERED_ACCESS_VIEW_DESC Desc;
	};
	/*
	Exp : �ؽ��� Ÿ�� 
	* �⺻
	* ť��  */
	enum class ETextureType
	{
		Default,
		Cube
	};
	/*
	�ؽ��� �� 
	* �ؽ��� ������ 
	* �ؽ��� Ÿ�� */
	struct TexturePack
	{
		std::wstring Path;
		std::shared_ptr<Texture> Texture;
		ETextureType Type;
	};
	/*
	���ҽ� �÷��� �� */
	struct ResourceFlagPack
	{
		D3D12_HEAP_PROPERTIES Property = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		D3D12_HEAP_FLAGS      Flags    = D3D12_HEAP_FLAG_NONE;
		D3D12_RESOURCE_STATES State    = D3D12_RESOURCE_STATE_COMMON;
	};

	/*
	Exp : RederTarget Pack */
	struct RenderTargetPack
	{
		std::string PackName;
		ID3D12Resource* RenderResource = nullptr;
		D3D12_RENDER_TARGET_VIEW_DESC Desc;
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle;
		bool bDefault = false;
	};
	/*
	Exp : DepthStencilView Pack */
	struct DepthStencilViewPack
	{
		std::string PackName;
		ID3D12Resource* RenderResource = nullptr;
		D3D12_DEPTH_STENCIL_VIEW_DESC Desc;
		CD3DX12_CPU_DESCRIPTOR_HANDLE Handle;
		bool bDefault = false;
	};
	class RCORE_EXPORT ResourceManager
	{
		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> m_Resources;
		std::unordered_map<ID3D12Resource*, D3D12_RESOURCE_STATES> m_ResourceStates;
	private:
		/*
		* Texture �迭
		* ShaderResourceView  �迭
		* UnorderedAccessView �迭 */
		std::unordered_map<std::wstring, TexturePack>         m_Textures;
		std::unordered_map<std::wstring, TexturePack>         m_CubeTextures;
		std::unordered_map<std::string, SrvResourcePack>      m_SrvResources;
		std::unordered_map<std::string, SrvResourcePack>      m_CubeSrvResources;
		std::unordered_map<std::string, UavResourcePack>      m_UavResources;
		std::unordered_map<std::string, RenderTargetPack>     m_RenderTargets;
		std::unordered_map<std::string, DepthStencilViewPack> m_DepthStencilViews;
		/*
		* Texture Offset  �迭
		* Srv,Uav Offset  �迭  */
		std::unordered_map<std::wstring, UINT> m_TextureOffsets;
		std::unordered_map<std::string,  UINT> m_SrvUavOffsets;
		std::unordered_map<std::string,  UINT> m_RtvOffsets;
		std::unordered_map<std::string,  UINT> m_DsvOffsets;
		std::unordered_map<std::wstring, UINT> m_CubeTextureOffsets;
		std::unordered_map<std::string,  UINT> m_CubeMapOffsets;
		/*
		*  �� ( srv, uav, texture ���� �����ϰ��ִ� ���� �迭 ) */
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_SrvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_RtvDescriptorHeap;
		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_DsvDescriptorHeap;
		/*
		* ���� �ʿ� ������ �� ������ */
		ID3D12Device* m_Device = nullptr;
		class DxCore* m_Core = nullptr;
		// ������ ���� ���� 
		UINT m_SrvHeapOffset  = 0;
		UINT m_CubeHeapOffset = 0;
		UINT m_RtvHeapOffset  = 0;
		UINT m_DsvHeapOffset  = 0;
	public:
		void Init(class DxCore* core);
	public:
		ID3D12Resource* BuildResource(D3D12_RESOURCE_DESC* desc, const ResourceFlagPack& Pack = ResourceFlagPack(), D3D12_CLEAR_VALUE* ClearValue = nullptr);
		ID3D12Resource* ReBuildResource(ID3D12Resource* resource, D3D12_RESOURCE_DESC* desc, const ResourceFlagPack& Pack = ResourceFlagPack(), D3D12_CLEAR_VALUE* ClearValue = nullptr);
	public:
		/*
		Exp : �ؽ���, SRV, UAV �� �߰� ���ִ� �Լ� */
		TexturePack* AddTexture(ID3D12GraphicsCommandList* CommandList, const std::wstring& path, ETextureType type = ETextureType::Default);
		SrvResourcePack* AddSrv(const std::string& name, ID3D12Resource* resource, D3D12_SHADER_RESOURCE_VIEW_DESC* Desc);
		UavResourcePack* AddUav(const std::string& name, ID3D12Resource* resource, ID3D12Resource* Counterresource, D3D12_UNORDERED_ACCESS_VIEW_DESC* Desc);
		RenderTargetPack* AddRtv(const std::string& name, ID3D12Resource* RenderResource, D3D12_RENDER_TARGET_VIEW_DESC* Desc);
		DepthStencilViewPack* AddDsv(const std::string& name, ID3D12Resource* RenderResource, D3D12_DEPTH_STENCIL_VIEW_DESC* Desc);
	public:
		SrvResourcePack* SetSrv(const std::string& name, ID3D12Resource* resource, D3D12_SHADER_RESOURCE_VIEW_DESC* Desc = nullptr);
		UavResourcePack* SetUav(const std::string& name, ID3D12Resource* resource, ID3D12Resource* Counterresource, D3D12_UNORDERED_ACCESS_VIEW_DESC* Desc = nullptr);
		RenderTargetPack* SetRtv(const std::string& name, ID3D12Resource* RenderResource, D3D12_RENDER_TARGET_VIEW_DESC* Desc = nullptr);
		DepthStencilViewPack* SetDsv(const std::string& name, ID3D12Resource* RenderResource, D3D12_DEPTH_STENCIL_VIEW_DESC* Desc = nullptr);
	public:
		void BuildResourceHeap();
	public:
		/*
		Exp : ���� ���ҽ� �� �ؽ���  Get������. */
		Texture* GetTexture(const std::wstring& path);
		ID3D12Resource* GetSrv(const std::string& name);
		ID3D12Resource* GetUrv(const std::string& name);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetRtvHandle(const std::string& name);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetDsvHandle(const std::string& name);
	public: 
		/*
		Exp : �� Get ������. */
		TexturePack* GetTexturePack(const std::wstring& path);
		SrvResourcePack* GetSrvPack(const std::string& name);
		UavResourcePack* GetUavPack(const std::string& name);
		RenderTargetPack* GetRtvPack(const std::string& name);
		DepthStencilViewPack* GetDsvPack(const std::string& name);
	public:
		/*
		Exp : Heap Offset Get ������ */
		UINT GetTextureIndex(const std::wstring& path);
		UINT GetSrvUavIndex(const std::string& name);
		UINT GetRtvIndex(const std::string& name);
		UINT GetDsvIndex(const std::string& name);
		UINT GetCubeMapShaderIndex(const std::string& name);
		UINT GetCubeTextureShaderIndex(const std::wstring& name);
	public:
		/*
		Exp : Heap Handle */
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUSrvUavHandle(const UINT HeapIndex);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUSrvUavHandle(const UINT HeapIndex);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPURtvHandle(UINT HeapIndex);
		CD3DX12_CPU_DESCRIPTOR_HANDLE GetCPUDsvHandle(UINT HeapIndex);
		/*
		Exp : TextureCube , Texture2D ������ Handle */
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUTexture2DHandle();
		CD3DX12_GPU_DESCRIPTOR_HANDLE GetGPUCubeMapHandle();
	public:
		/*
		Exp : DescriptorHeap ������ */
		ID3D12DescriptorHeap* SrvHeap() { return m_SrvDescriptorHeap.Get(); }
	public:
		UINT GetTextureCount() { return (UINT)m_Textures.size(); }
	public:
		void Clear();
		bool ResourceStateTransition(ID3D12GraphicsCommandList* CommandList, ID3D12Resource* resource, D3D12_RESOURCE_STATES State);
	private:
		/*
		EXP : �ؽ��� ��� �ۼ� */
		static D3D12_SHADER_RESOURCE_VIEW_DESC GetDesc(TexturePack* t);
	};

}