#pragma once

#include"DirectXCommon/DirectXCommon.h"

namespace Dx12
{
	class Dx12CommonShaderDefines;
	class DescriptorAllocation;
	class CommandQueue;
	namespace DxDevice
	{
		// DirectX12 ��ġ ����
		bool CreateDevice();


		// DirectX12 ��ġ ����
		void DestroyDevice();


		/* Descriptor �� �Ҵ� �ϱ�
		@param type : DescriptorHeap Ÿ�� 
		@param numDescriptors : �Ҵ��� Descriptor �� ����  */
		DescriptorAllocation AllocateDescriptors(D3D12_DESCRIPTOR_HEAP_TYPE type, uint32_t numDescriptors = 1);



		// CommandQueue ���
		// @param type : CommandQeueu Ÿ��
		CommandQueue* GetCommandQueue(D3D12_COMMAND_LIST_TYPE type = D3D12_COMMAND_LIST_TYPE_DIRECT);



		// ���� Shader ���� Ŭ���� ���
		Dx12CommonShaderDefines* GetShaderCommonDefines();


		// ����ϰ� ������ DescriptorHeap ���� 
		// @param finishedFrame : ��ġ�� ī��Ʈ�Ǿ������ִ� Frame
		void ReleaseStaleDescriptors(uint64_t finishedFrame);

		// ��ġ �� ī��Ʈ ����
		void NewFrame();

		// ��� Ÿ���� CommandQueue ��� ����Ʈ ����
		void Flush();


		// ID3D12Device* ��������
		ID3D12Device*  GetDevice();

		// IDXGIFactory4* ��������
		IDXGIFactory4* GetFactory();

		// IDXGIAdapter ����Ʈ �������� 
		std::vector<IDXGIAdapter*>    GetAdapter();

		// DescriptorHeap �ϳ��� ũ�� ������
		// @param type : DescriptorHeap Ÿ��
		UINT GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE type);

		// ��ġ�� ī��Ʈ ��������
		uint32_t  GetFrameCount();

		// Tearing ����� �����Ǵ��� ����
		bool      IsTearingSupport();
	}

}

