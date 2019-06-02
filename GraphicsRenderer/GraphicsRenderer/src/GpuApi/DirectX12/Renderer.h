#pragma once
#include"Dx12Include.h"
#include"GraphicsResource.h"
#include"Viewport.h"
#include"ScissorRect.h"
#include"RootSignature.h"
#include"RenderTarget.h"
#include"PipelineState.h"
#include"ShaderType.h"

namespace GR
{
	namespace Dx12
	{
		// 
		class GraphicsDevice;
		class GraphicsCommander;
		class Camera;
		class Mesh;
		class PSOCache;
		class RootSignatureCache;
		class RenderObject;

		/* Renderer ��ɿ� �ʿ��Ѱ�




		RenderObject  GetSRV ��� ������Ʈ �ɶ��� HANDLE �� ����
		HDR �ؽ��� �ε� �ٽ� ������ ( HDR �������� ������ ��� ���̴� ����)

		*/

		class Renderer
		{
		private:
			enum ERenderPass
			{
				GBUFFER,
				SKYBOX,
				NUMPASS
			};
		private:
			friend GraphicsDevice;
			struct RenderPass
			{
				RenderTarget RT;
				PassCB       passCB;
				Viewport     viewPort;
				ScissorRect  scissorRect;
			};
		private:
			GraphicsDevice* m_GraphcisDevice;
	
			std::shared_ptr<Camera> m_Camera;
			std::shared_ptr<RenderObject> m_SkyBox;


			RootSignature m_GCommonRootSignature;
			//
		public:
			RenderPass m_GBufferPass;
			std::map<ERenderPass, GraphicsCommander*> m_PassCommanders;
		public:
			Renderer();
			GraphicsDevice* GetDevice();
			const GraphicsDevice* GetDevice() const;

			void Initialize(uint32_t width, uint32_t height);
			Camera* GetCamera();
		public:
			void RenderBegin();
			void RenderEnd();
		public: // GBuffer
			void GBufferOn();
			void GBufferRender(const RenderObject& obj);
			void GBufferOff();
			RenderTarget* GetGBufferRenderTarget();
		public:
			void SkyBoxRender(const Texture& texture);
		private:
			void GBufferInit(uint32_t width, uint32_t height);
			void BakeIBLTexture(ComputeCommander* commander, const Texture& inTexture, Texture& outSpMap, Texture& outSpbrdf, Texture& irrMap);
		public:

		};



	}
}