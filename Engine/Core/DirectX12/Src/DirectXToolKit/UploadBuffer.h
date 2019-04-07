#pragma once
#include"DirectXCommon/DirectXCommon.h"

namespace Dx12
{
	class UploadBuffer
	{
	public:
		// CPU �ּҿ� GPU �ּҰ� ����ִ� ����ü 
		struct Allocation
		{
			void* CPU;
			D3D12_GPU_VIRTUAL_ADDRESS GPU;
		};
	private:
		class Page
		{
			ComPtr<ID3D12Resource> m_D3D12Resource;
			//
			void* m_CPUptr;
			D3D12_GPU_VIRTUAL_ADDRESS m_GPUptr;
			//
			size_t m_PageSize;
			size_t m_Offset;
		public:
			Page(size_t sizeInBytes);
			~Page();

			/* 
			���� ��û�� �޸� �Ҵ��Ҽ� �ִ� ���� ������ �ִ��� üũ ���ش�. */
			bool HasSpace(size_t sizeInBytes, size_t alignment) const;

			/*
			������ ������ �Ҵ��Ѵ�.*/
			Allocation Allocate(size_t sizeInBytes, size_t alignment);

			// ������ ����
			void Reset();
			
		};
		using PagePool = std::deque<std::shared_ptr<Page>>;

		PagePool m_PagePool;
		PagePool m_AvailablePages;

		std::shared_ptr<Page> m_CurrentPage;

		size_t m_PageSize;

	public:
		explicit UploadBuffer(size_t pageSize = _2MB);
		virtual ~UploadBuffer();
	public:
		size_t GetPageSize() const { return m_PageSize; }
		Allocation Allocate(size_t sizeInBytes, size_t alignment);
		void Reset();
	private:
		// �޸� ��û�� ������ �̹� �Ҵ�ǰ� ������ ���������� �Ҵ�ǰ� ���� ���ٸ�
		// ���� �������� �ۼ��Ѵ�.
		std::shared_ptr<Page> RequestPage();
	};
}