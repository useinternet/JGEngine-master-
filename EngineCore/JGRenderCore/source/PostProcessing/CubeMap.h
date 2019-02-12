#pragma once
#include"DxCommon/DxCommon.h"
namespace JGRC
{
	class Scene;
	class RCORE_EXPORT CubeMap
	{
		CubeMap(const CubeMap& rhs) = delete;
		CubeMap& operator=(const CubeMap& rhs) = delete;
	private:
		static UINT m_Width;
		static UINT m_Height;
	public:
		std::string m_ObjName;
		D3D12_VIEWPORT m_ViewPort;
		D3D12_RECT m_ScissorRect;
		DXGI_FORMAT     m_Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		ID3D12Resource* m_CubeMap = nullptr;
		ID3D12Resource* m_DepthStencilBuffer = nullptr;
		// ť�� ī�޶�
		Camera m_CubeCamera[6];
		// ���� Ÿ�� �� ���� ���ٽ� ��
		RenderTargetPack* RtvPack[6];
		DepthStencilViewPack* DsvPack;
		// �о� ������
		PassData* m_CubeMapPass[6];
		// ���� �н�
		std::shared_ptr<class SceneData> m_CubeScene;
	public:
		CubeMap() = default;
		CubeMap(UINT Width, UINT Height);
		void BuildCubeMap(const std::string& objName, ID3D12GraphicsCommandList* CommandList);
		void Update(const GameTimer& gt, FrameResource* CurrentResource);
		void Excute(FrameResource* CurrentResource, ID3D12GraphicsCommandList* CommandList, bool isDynamic = true);
		UINT GetCubeMapIndex();
	private:
		void BuildDescriptors();
		void BuildResource(ID3D12GraphicsCommandList* CommandList);
	public:
		void BuildCamera(float x, float y, float z);
	};
}