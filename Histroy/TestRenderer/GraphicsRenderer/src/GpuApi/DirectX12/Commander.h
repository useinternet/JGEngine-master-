#pragma once
#include"Dx12Include.h"

namespace GR
{
	namespace Dx12
	{
		class GPUResource;
		class GPUBuffer;
		class Texture;
		class DepthTexture;
		class ColorTexture;
		class UploadBuffer;
		class Viewport;
		class ScissorRect;
		class VertexBuffer;
		class IndexBuffer;
		class RootSignature;
		class GraphicsPSO;
		class ComputePSO;
		class GraphicsCommander;
		class ComputeCommander;
		class GraphicsDevice;
		class Commander
		{
			friend GraphicsDevice;
		protected:
			static std::wstring ms_GenerateMipMapsShaderPath;
		protected:
			class GPUDescriptorAllocator
			{
			public:
				enum EHeapType
				{
					SRV,
					UAV,
					CBV,
					NUMTYPE
				};
				struct CPUDescriptorHandle
				{
					uint32_t rootIndex;
					uint32_t numHandles;
					std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles;
				};

			private:
				ComPtr<ID3D12DescriptorHeap>             m_GPUHeap;
				D3D12_CPU_DESCRIPTOR_HANDLE              m_StartCPUHeapHandle;
				D3D12_GPU_DESCRIPTOR_HANDLE              m_StartGPUHeapHandle;
				uint32_t                                 m_IncrementDescritporSize;
				D3D12_DESCRIPTOR_HEAP_TYPE               m_D3D_DescirptorHeapType;
				ID3D12Device* m_D3DDevice;
				std::vector<CPUDescriptorHandle>         m_CPUCache[NUMTYPE];
				SIZE_T                                   m_Offset[NUMTYPE];
			public:
				GPUDescriptorAllocator(ID3D12Device* device);
				// �� ���� ���� Heap �޸�
				void StageDescriptorHandles(EHeapType type, uint32_t rootIndex, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
				// �������� Heap �޸�
				void StageDescriptorHandles(EHeapType type,uint32_t rootIndex, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE handle);
				void BindDescriptorTable(Commander* commander, bool is_compute);
				void BindDescriptorHeap(Commander* commander);
				void Reset();
				void Flush();
			};
			Commander(const Commander&) = delete;
			Commander& operator=(const Commander&) = delete;
		protected:
			D3D12_COMMAND_LIST_TYPE                m_CmdListType;
			ComPtr<ID3D12GraphicsCommandList>      m_D3D_CommandList;
			ComPtr<ID3D12CommandAllocator>         m_D3D_CommandAllocator;
			ComPtr<ID3D12Device>                   m_D3DDevice;
			//
			std::shared_ptr<GPUDescriptorAllocator> m_GPUAllocator;
			std::shared_ptr<UploadBuffer>       m_UploadBuffer;
			std::vector<ComPtr<ID3D12Object>>   m_BackedUpObject;
			std::vector<D3D12_RESOURCE_BARRIER> m_ResourceBarriers;
			//
		protected:
			ID3D12PipelineState* m_CurrPipeLineState;
			ID3D12RootSignature* m_CurrRootSignature;
			std::shared_ptr<ComputePSO> m_GenerateMipMapsPSO;
		public:
			Commander(ComPtr<ID3D12Device> device, D3D12_COMMAND_LIST_TYPE type);
			ID3D12GraphicsCommandList* GetD3DCommandList() const {
				return m_D3D_CommandList.Get();
			}
			virtual ~Commander() {}

			// -- ���ҽ� �庮 -- //
			void TransitionBarrier(GPUResource& resource, D3D12_RESOURCE_STATES newState, bool is_flush = false);
			void BeginTransitionBarrier(GPUResource& resource, D3D12_RESOURCE_STATES newState, bool is_flush = false);
			void UAVBarrier(GPUResource& Resource, bool is_flush = false);
			void AliasBarrier(GPUResource& Before, GPUResource& After, bool is_flush = false);
			void FlushResourceBarriers();


			// -- ���� �Լ� -- //

			void CopyResource(GPUResource& dest, GPUResource& src);
			void CopyResourceRegion(
				GPUResource& dest, uint32_t dstMip, uint32_t dstX, uint32_t dstY, uint32_t dstZ,
				GPUResource& src, uint32_t srcMip, uint32_t arraySlice);
			void CopyBuffer(
				GPUBuffer& buffer, 
				uint32_t numElements, uint32_t elementSize, 
				const void* bufferData,
				D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);
			// CopyResourceRegion
			// CopySubresource
			// CopyCounter
			// ResetCounter
		

			


			// -- CommandList �޸� ���� ���� -- //
			void BackUpObject(const GPUResource& resource);
			void BackUpObject(ComPtr<ID3D12Object> obj);
			void FlushBackUpObject();
			void Close();
			void Reset();


		};

		class GraphicsCommander : public Commander
		{

		public:
			GraphicsCommander(ComPtr<ID3D12Device> device) :
				Commander(device, D3D12_COMMAND_LIST_TYPE_DIRECT) {}
			virtual ~GraphicsCommander() {}
		public:
			void ClearColor(ColorTexture& texture);
			void ClearDepth(DepthTexture& texture);
			void ClearStencil(DepthTexture& texture);
			void ClearDepthStencil(DepthTexture& texture);


			void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE RTV, D3D12_CPU_DESCRIPTOR_HANDLE DSV);
			void SetRenderTarget(D3D12_CPU_DESCRIPTOR_HANDLE RTV);
			void SetRenderTargets(uint32_t numRtv, const D3D12_CPU_DESCRIPTOR_HANDLE RTV[], D3D12_CPU_DESCRIPTOR_HANDLE DSV);
			void SetRenderTargets(uint32_t numRtv, const D3D12_CPU_DESCRIPTOR_HANDLE RTV[]);


			void SetViewport(const Viewport& viewport);
			void SetViewports(uint32_t numViewport, Viewport* viewport);

			void SetScissorRect(const ScissorRect& rect);
			void SetScissorRects(uint32_t numScissorRect, ScissorRect* rect);

			void SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY topology);
			
	
			void SetConstants(uint32_t rootparam, uint32_t num32BitValues, const void* srcData);
			void SetConstantBuffer(uint32_t rootparam,  GPUResource& resource);
			void SetDynamicConstantBuffer(uint32_t rootparam, uint32_t sizeInByte, const void* data);


			void SetDynamicStructuredBuffer(uint32_t rootparam, uint32_t numElement, uint32_t elementSize, const void* data);
			void SetShaderResourceView(uint32_t rootparam, GPUResource& resource);

			
			void SetVertexBuffer(VertexBuffer& buffer);
			void SetDynamicVertexBuffer(uint32_t numVertices, uint32_t vertexStride, const void* data);
			void SetIndexBuffer(IndexBuffer& buffer);
			void SetDynamicIndexBuffer(uint32_t indexCount, uint32_t elementSize, const void* data);



			void SetPipelineState(GraphicsPSO& pso);
			void SetRootSignature(RootSignature& rootsig);

			void SetSRVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			void SetUAVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			void SetCBVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			// Draw ��
			void Draw(uint32_t vertexCount, uint32_t instanceCount = 1, uint32_t startVertexLocation = 0, uint32_t startInstanceLocation = 0);
			void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t startIndexLocation = 0, uint32_t baseVertexLocation = 0, uint32_t startInstanceLocation = 0);


			class ComputeCommander*  ConvertComputeCommander();
		};


		class ComputeCommander : public Commander
		{
		public:
			ComputeCommander(ComPtr<ID3D12Device> device) :
				Commander(device, D3D12_COMMAND_LIST_TYPE_COMPUTE) {}
			virtual ~ComputeCommander() {}
		public:
			void SetConstants(uint32_t rootparam, uint32_t num32BitValues, const void* srcData);
			void SetConstantBuffer(uint32_t rootparam, GPUResource& resource);
			void SetDynamicConstantBuffer(uint32_t rootparam, uint32_t sizeInByte, const void* data);


			void SetDynamicStructuredBuffer(uint32_t rootparam, uint32_t numElement, uint32_t elementSize, const void* data);
			void SetShaderResourceView(uint32_t rootparam, GPUResource& resource);
			void SetPipelineState(ComputePSO& pso);
			void SetRootSignature(RootSignature& rootsig);

	
			void SetSRVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			void SetUAVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			void SetCBVDescriptorTable(uint32_t rootparam, uint32_t numHandles, D3D12_CPU_DESCRIPTOR_HANDLE* handle);
			void Dispatch(uint64_t groupX, uint64_t groupY, uint64_t groupZ);
		public:
			void GenerateMipMaps(Texture& texture);
		private:
			void GenerateMipMaps_UAV(Texture& texture);
		};

		class CopyCommander : public Commander
		{
		public:
			CopyCommander(ComPtr<ID3D12Device> device) : 
				Commander(device,D3D12_COMMAND_LIST_TYPE_COPY) {}
			virtual ~CopyCommander() {}

		};
	}
}
