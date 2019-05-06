#pragma once
#include"CubeMap.h"


namespace Dx12
{
	class GBuffer;
	class Scene;
	class CommandList;
	class Dx12Object;
	class ReflectionMap : public CubeMap
	{
	private:
		std::unique_ptr<GBuffer> m_GBuffer;
		std::unique_ptr<Scene>   m_Scene[6];
		bool m_EnableMipMaps;
	public:
		/* ť��� ������
		@param width,height : ť��� ũ�� 
		@param farZ, NearZ  : �ּҰŸ�, �ִ� �Ÿ�,
		@param format : ť��� Format */
		ReflectionMap(
			uint32_t width, uint32_t height,
			float farZ, float NearZ , bool enableMipmap = false,
			DXGI_FORMAT format = DXGI_FORMAT_R8G8B8A8_UNORM);


		// ť�� �� �׸���
		// @param commandList : commandList Ŭ����
		// @param objArray : �׸� ������Ʈ �迭(vector Ÿ��)
		void Draw(CommandList* commandList, const std::vector<Dx12Object*>& objArray);

		// ũ�� �缳��
		// @param width,height : ������ ť��� ũ��
		void Resize(uint32_t width, uint32_t height);

		// CubeMap �ؽ��� ��������
		virtual const Texture& GetTexture() const override;
	protected:
		virtual void Build() override;
		void ResourceBuild();
	};
}