#include "pch.h"
#include "DescriptorHeap.h"
#include "Class/Log.h"
#include "DescriptorHandle.h"
#include "DescriptorAllocator.h"
#include "DxUtill.h"
#include "RenderDevice.h"

using namespace std;

namespace RE
{
	DescriptorHeap::DescriptorHeap(DescriptorAllocator* owner_allocator, uint32_t numDescriptor)
		: DxObject("DescriptorHeap"),
		m_OwnerAllcator(owner_allocator),
		m_NumDescriptor(numDescriptor)
	{
		m_D3D_Heap = CreateD3DDescriptorHeap(GetD3DDevice(), m_OwnerAllcator->GetType(), D3D12_DESCRIPTOR_HEAP_FLAG_NONE, m_NumDescriptor);
		m_IncreasementSize = GetD3DDevice()->GetDescriptorHandleIncrementSize(m_OwnerAllcator->GetType());
		
		DescriptorBlock block;
		block.startPos = 0;
		block.endPos = numDescriptor - 1;
		block.numDescriptor = numDescriptor;
		block.isAllocate = false;
		m_DescriptorBlocks.emplace(block.startPos, block);
	}

	bool DescriptorHeap::HasSpace(uint32_t numDescriptor) const
	{
		for (auto& block : m_DescriptorBlocks)
		{
			if (block.second.HasSpace(numDescriptor))
			{
				return true;
			}
		}

		return false;
	}
	
	DescriptorHandle DescriptorHeap::Allocate(uint32_t numDescriptor)
	{
		lock_guard<mutex> lock(m_Mutex);
		uint32_t key = -1;
		for (auto& block : m_DescriptorBlocks)
		{
			if (block.second.HasSpace(numDescriptor))
			{
				key = block.first;
				break;
			}
		}

		if (key == -1)
			return DescriptorHandle(nullptr, -1);


		uint64_t device_frameCount = GetRenderDevice()->GetFrameCount();
		uint64_t device_ValueIndex = GetRenderDevice()->GetValueIndex();


		// ���� �Ҵ��ҷ��� ũ��� ���� ���� ũ�Ⱑ ���ٸ� �׳� �Ҵ��� ����
		if (numDescriptor == m_DescriptorBlocks[key].numDescriptor)
		{
			// �� �Ҵ�
			m_DescriptorBlocks[key].isAllocate = true;
			m_DescriptorBlocks[key].frameCount = device_frameCount;
			m_DescriptorBlocks[key].valueIndex = device_ValueIndex;

			uint32_t block_key = m_DescriptorBlocks[key].startPos;

			D3D12_CPU_DESCRIPTOR_HANDLE Cpu =
			{ m_D3D_Heap->GetCPUDescriptorHandleForHeapStart().ptr +
			 (m_DescriptorBlocks[key].startPos * m_IncreasementSize) };

			DescriptorHandle handle(this, block_key);
			handle.m_CPU = Cpu;
			// �޸� �Ҵ��� return
			return move(handle);
		}



		auto curr_pos = m_DescriptorBlocks.find(key);
		auto prev_pos = curr_pos;
		--prev_pos;

		auto next_pos = curr_pos;
		++next_pos;

		DescriptorBlock empty_block;
		DescriptorBlock allocate_block;

		empty_block.frameCount = device_frameCount;
		empty_block.valueIndex = device_ValueIndex;
		allocate_block.frameCount = device_frameCount;
		allocate_block.valueIndex = device_ValueIndex;

		uint32_t deltaNumDescriptor = curr_pos->second.numDescriptor - numDescriptor;
		// �� ó�� ����� ���
		if (m_DescriptorBlocks.size() == 1)
		{
			// �� �տ� �Ҵ�
			allocate_block.startPos = curr_pos->second.startPos;
			allocate_block.endPos = allocate_block.startPos + numDescriptor - 1;
			allocate_block.isAllocate = true;
			allocate_block.numDescriptor = numDescriptor;

			empty_block.startPos = allocate_block.endPos + 1;
			empty_block.numDescriptor = deltaNumDescriptor;
			empty_block.endPos = empty_block.startPos + empty_block.numDescriptor - 1;
			empty_block.isAllocate = false;

			m_DescriptorBlocks.erase(curr_pos);
			m_DescriptorBlocks.emplace(empty_block.startPos, empty_block);
			m_DescriptorBlocks.emplace(allocate_block.startPos, allocate_block);
		}
		else if (curr_pos == m_DescriptorBlocks.begin())
		{
			// �ǵڿ� �Ҵ� ��
			empty_block.startPos = curr_pos->second.startPos;
			empty_block.endPos = empty_block.startPos + deltaNumDescriptor - 1;
			empty_block.isAllocate = false;
			empty_block.numDescriptor = deltaNumDescriptor;



			allocate_block.startPos = empty_block.endPos + 1;
			allocate_block.endPos = allocate_block.startPos + numDescriptor - 1;
			allocate_block.isAllocate = true;
			allocate_block.numDescriptor = numDescriptor;


			m_DescriptorBlocks.erase(curr_pos);
			m_DescriptorBlocks.emplace(empty_block.startPos, empty_block);
			m_DescriptorBlocks.emplace(allocate_block.startPos, allocate_block);
		}
		// �� �� ��� �� ���
		else if (curr_pos == --m_DescriptorBlocks.end())
		{
			// �տ� �Ҵ� ��� 
			allocate_block.startPos = curr_pos->second.startPos;
			allocate_block.endPos = allocate_block.startPos + numDescriptor - 1;
			allocate_block.isAllocate = true;
			allocate_block.numDescriptor = numDescriptor;

			empty_block.startPos = allocate_block.endPos + 1;
			empty_block.numDescriptor = deltaNumDescriptor;
			empty_block.endPos = empty_block.startPos + empty_block.numDescriptor - 1;
			empty_block.isAllocate = false;


			m_DescriptorBlocks.erase(curr_pos);
			m_DescriptorBlocks.emplace(allocate_block.startPos, allocate_block);
			m_DescriptorBlocks.emplace(empty_block.startPos, empty_block);
		}
		// �߰� ����� ���
		else
		{
			auto prev_block = prev_pos->second;
			auto curr_block = curr_pos->second;
			auto next_block = next_pos->second;
			// �տ� �Ҵ� ��� 
			allocate_block.startPos = curr_pos->second.startPos;
			allocate_block.endPos = allocate_block.startPos + numDescriptor - 1;
			allocate_block.isAllocate = true;
			allocate_block.numDescriptor = numDescriptor;

			empty_block.startPos = allocate_block.endPos + 1;
			empty_block.numDescriptor = deltaNumDescriptor;
			empty_block.endPos = empty_block.startPos + empty_block.numDescriptor - 1;
			empty_block.isAllocate = false;


			m_DescriptorBlocks.erase(curr_pos);
			m_DescriptorBlocks.emplace(allocate_block.startPos, allocate_block);
			m_DescriptorBlocks.emplace(empty_block.startPos, empty_block);
		}


		uint32_t block_key = allocate_block.startPos;

		D3D12_CPU_DESCRIPTOR_HANDLE Cpu =
		{ m_D3D_Heap->GetCPUDescriptorHandleForHeapStart().ptr + 
		 (allocate_block.startPos * m_IncreasementSize) };

		DescriptorHandle handle(this, block_key);
		handle.m_CPU = Cpu;
		return std::move(handle);
	}
	void DescriptorHeap::Free(DescriptorHandle* handle)
	{
		lock_guard<mutex> lock(m_Mutex);
		auto block = m_DescriptorBlocks[handle->m_Block_Key];

		m_FreeReservatedBlocks[block.valueIndex].push_back(block);

		handle->m_Block_Key = -1;
		handle->m_CPU = { 0 };
		handle->m_pOwnerHeap = nullptr;
	}
	void DescriptorHeap::RenewalHandle(const DescriptorHandle* handle)
	{
		auto key = handle->m_Block_Key;

		m_DescriptorBlocks[key].frameCount = GetRenderDevice()->GetFrameCount();
		m_DescriptorBlocks[key].valueIndex = GetRenderDevice()->GetValueIndex();
	}
	void DescriptorHeap::ReleaseDescriptorBlock()
	{
		uint64_t frameCount = GetRenderDevice()->GetFrameCount();
		uint64_t valueIndex = GetRenderDevice()->GetValueIndex();

		auto& block_array = m_FreeReservatedBlocks[valueIndex];
		uint32_t array_size = (uint32_t)block_array.size();

		
		for (uint32_t i = 0; i < array_size; ++i)
		{
			if (frameCount == block_array[i].frameCount)
			{
				continue;
			}
			if (i == array_size - 1)
			{
				FreeBlock(block_array[i]);
				block_array.pop_back();
				break;
			}
			else
			{
				FreeBlock(block_array[i]);

				auto temp = block_array[i];
				block_array[i] = block_array[array_size - 1];
				block_array[array_size - 1] = temp;
				block_array.pop_back();
				--array_size;
				--i;
			}
		}
	}
	void DescriptorHeap::GetDebugInfo(Debug::DescriptorHeapInfo& out_debug_info)
	{
		lock_guard<mutex> lock(m_Mutex);

		uint32_t allocatedDescriptorCount = 0;
		for (auto& block_pair : m_DescriptorBlocks)
		{
			auto& block = block_pair.second;
			Debug::DescriptorBlockInfo block_debug_info;
			block_debug_info.address = m_D3D_Heap->GetCPUDescriptorHandleForHeapStart().ptr + (block.startPos * m_IncreasementSize);
			block_debug_info.is_allocate = block.isAllocate;
			block_debug_info.numDescriptor = block.numDescriptor;
			block_debug_info.start_offset = block.startPos;
			out_debug_info.descriptor_blocks[block_debug_info.start_offset] = block_debug_info;


			if (block.isAllocate)
				allocatedDescriptorCount++;
		}
		out_debug_info.max_numDescriptor = m_NumDescriptor;
		out_debug_info.num_allocated_descriptor = allocatedDescriptorCount;
	}
	void DescriptorHeap::FreeBlock(const DescriptorBlock& block)
	{
		auto curr_pos = m_DescriptorBlocks.find(block.startPos);
		auto prev_pos = curr_pos;
		--prev_pos;

		auto next_pos = curr_pos;
		++next_pos;

		
		// ���� ���� �Ǿտ� �ִ� ���
		if (curr_pos == m_DescriptorBlocks.begin())
		{
			// ���� ���� �Ҵ�� ��� �Ҵ� ����
			if (next_pos->second.isAllocate)
			{
				m_DescriptorBlocks[curr_pos->second.startPos].isAllocate = false;
			}
			// ���� ���� �Ҵ� �ȉ��� ��� ���� ���� ����
			else
			{
				auto new_block = MergeBlock(curr_pos->second, next_pos->second, false);
				m_DescriptorBlocks.erase(curr_pos);
				m_DescriptorBlocks.erase(next_pos);
				m_DescriptorBlocks.emplace(new_block.startPos, new_block);

			}
		}

		// ���� ���� �ǵڿ� �ִ� ���
		else if (curr_pos == --m_DescriptorBlocks.end())
		{
			if (prev_pos->second.isAllocate)
			{
				m_DescriptorBlocks[curr_pos->second.startPos].isAllocate = false;
			}
			else
			{
				auto new_block = MergeBlock(prev_pos->second, curr_pos->second, false);
				m_DescriptorBlocks.erase(curr_pos);
				m_DescriptorBlocks.erase(prev_pos);
				m_DescriptorBlocks.emplace(new_block.startPos, new_block);
			
			}

		}
		// ���� ���� �߾ӿ� �ִ� ���
		else
		{
			auto curr_block = curr_pos->second;
			auto prev_block = prev_pos->second;
			auto next_block = next_pos->second;

			// �� ���� �Ҵ�Ǿ��ְ� �������� �Ҵ� �ȵ� ���
			if (prev_block.isAllocate && !next_block.isAllocate)
			{
				auto new_block = MergeBlock(curr_block, next_block, false);
				m_DescriptorBlocks.erase(curr_block.startPos);
				m_DescriptorBlocks.erase(next_block.startPos);
				m_DescriptorBlocks.emplace(new_block.startPos, new_block);
			}
			// �� ���� �Ҵ� �ȵǾ��ְ� ���� ���� �Ҵ�Ǿ��������
			else if (!prev_block.isAllocate && next_block.isAllocate)
			{
				auto new_block = MergeBlock(prev_block, curr_block, false);
				m_DescriptorBlocks.erase(curr_block.startPos);
				m_DescriptorBlocks.erase(prev_block.startPos);
				m_DescriptorBlocks.emplace(new_block.startPos, new_block);
			}
			// ���� ���� �� ��� �Ҵ� �ȵǾ����� ���
			else if (!prev_block.isAllocate && !next_block.isAllocate)
			{
				DescriptorBlock new_block;

				new_block.startPos = prev_block.startPos;
				new_block.endPos = next_block.endPos;
				new_block.isAllocate = false;
				new_block.numDescriptor =
					prev_block.numDescriptor + curr_block.numDescriptor + next_block.numDescriptor;

				m_DescriptorBlocks.erase(prev_block.startPos);
				m_DescriptorBlocks.erase(curr_block.startPos);
				m_DescriptorBlocks.erase(next_block.startPos);

				m_DescriptorBlocks.emplace(new_block.startPos, new_block);
			}
			// ���� ���� �ҷ� ��� �Ҵ� �Ǿ����� ���
			else
			{
				m_DescriptorBlocks[curr_pos->second.startPos].isAllocate = false;
			}
		}
	}
	DescriptorHeap::DescriptorBlock DescriptorHeap::MergeBlock(
		const DescriptorBlock& block1, 
		const DescriptorBlock& block2,
		bool is_allocate)
	{
		DescriptorBlock new_block;

		new_block.startPos = std::min(block1.startPos, block2.startPos);
		new_block.endPos = std::max(block1.endPos, block2.endPos);
		new_block.isAllocate = is_allocate;
		new_block.numDescriptor = block1.numDescriptor + block2.numDescriptor;


		return new_block;
	}
}