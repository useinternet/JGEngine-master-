#include "pch.h"
#include "ResourceDataMap.h"
#include "Resource.h"
#include "CommandList.h"


namespace RE
{
	std::unordered_map<ID3D12Resource*, _ResourceData> ResourceDataMap::ms_GlobalResourceDatas;
	std::mutex ResourceDataMap::ms_GlobalStateMutex;
	std::mutex ResourceDataMap::ms_GlobalRegisterMutex;
	std::mutex ResourceDataMap::ms_GlobalDataMutex;
	bool ResourceDataMap::ms_IsStateLocked;
	bool ResourceDataMap::ms_IsDataLocked;



	ResourceDataMap::ResourceDataMap() : DxObject("ResourceDataMap")
	{

	}
	void ResourceDataMap::TransitionBarrier(const Resource& resource, D3D12_RESOURCE_STATES after_state,
		uint32_t subresource)
	{
		if (resource.IsVaild())
		{
			ResourceBarrier(CD3DX12_RESOURCE_BARRIER::Transition(resource.GetD3DResource(),
				D3D12_RESOURCE_STATE_COMMON, after_state, subresource));
		}
	}
	void ResourceDataMap::UavBarrier(const Resource& resource)
	{
		if (resource.IsVaild())
		{
			ResourceBarrier(CD3DX12_RESOURCE_BARRIER::UAV(resource.GetD3DResource()));
		}
		else
		{
			ResourceBarrier(CD3DX12_RESOURCE_BARRIER::UAV(nullptr));
		}
	}
	void ResourceDataMap::AliasBarrier(const Resource& before_resource, const Resource& after_resource)
	{
		ID3D12Resource* before_d3d_resource =
			(before_resource.IsVaild()) ? before_resource.GetD3DResource() : nullptr;
		ID3D12Resource* after_d3d_resource =
			(after_resource.IsVaild()) ? after_resource.GetD3DResource() : nullptr;

		ResourceBarrier(CD3DX12_RESOURCE_BARRIER::Aliasing(before_d3d_resource, after_d3d_resource));
	}
	
	void ResourceDataMap::FlushResourceBarrier(CommandList* commandList)
	{
		if (m_ResourceBarriers.empty())
			return;


		auto d3d_cmdList = commandList->GetD3DCommandList();

		d3d_cmdList->ResourceBarrier((uint32_t)m_ResourceBarriers.size(), m_ResourceBarriers.data());
		m_ResourceBarriers.clear();
	}
	uint32_t ResourceDataMap::FlushPendingResourceBarrier(CommandList* pendingCommandList)
	{
		assert(ms_IsStateLocked);

		std::vector<D3D12_RESOURCE_BARRIER> resourceBarriers;
		resourceBarriers.reserve(m_PendingResourceBarriers.size());


		for (auto pendingBarrier : m_PendingResourceBarriers)
		{

			if (pendingBarrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
			{
				auto pendingTransition = pendingBarrier.Transition;


				const auto& iter = ms_GlobalResourceDatas.find(pendingTransition.pResource);

				if (iter != ms_GlobalResourceDatas.end())
				{
					auto& resourceState = iter->second.ResourceState;
					if (pendingTransition.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES &&
						!resourceState.subResourceDataMap.empty())
					{
						for (auto subresourceState : resourceState.subResourceDataMap)
						{
							if (pendingTransition.StateAfter != subresourceState.second)
							{
								D3D12_RESOURCE_BARRIER newBarrier = pendingBarrier;
								newBarrier.Transition.Subresource = subresourceState.first;
								newBarrier.Transition.StateBefore = subresourceState.second;
								resourceBarriers.push_back(newBarrier);
							}
						}


					}
					else
					{
						auto globalState = (iter->second.ResourceState).GetResourceState(pendingTransition.Subresource);
						if (pendingTransition.StateAfter != globalState)
						{
							pendingBarrier.Transition.StateBefore = globalState;
							resourceBarriers.push_back(pendingBarrier);
						}
					}
				}
			}

		}


		uint32_t numBarriers = (uint32_t)resourceBarriers.size();

		if (numBarriers > 0)
		{
			auto d3d_cmdList = pendingCommandList->GetD3DCommandList();
			d3d_cmdList->ResourceBarrier(numBarriers, resourceBarriers.data());
		}

		return numBarriers;
	}
	void ResourceDataMap::CommitResourceState()
	{
		assert(ms_IsStateLocked);

		for (const auto& resourceState : m_FinalResourceState)
		{
			ms_GlobalResourceDatas[resourceState.first].ResourceState = resourceState.second;
		}
		m_FinalResourceState.clear();
	}
	void ResourceDataMap::Reset()
	{
		m_PendingResourceBarriers.clear();
		m_ResourceBarriers.clear();
		m_FinalResourceState.clear();
	}
	void ResourceDataMap::StateLock()
	{
		ms_GlobalStateMutex.lock();
		ms_IsStateLocked = true;
	}
	void ResourceDataMap::StateUnLock()
	{
		ms_GlobalStateMutex.unlock();
		ms_IsStateLocked = false;
	}
	void ResourceDataMap::DataLock()
	{
		ms_GlobalDataMutex.lock();
		ms_IsDataLocked = true;
	}
	void ResourceDataMap::DataUnLock()
	{
		ms_GlobalDataMutex.unlock();
		ms_IsDataLocked = false;
	}
	void ResourceDataMap::RegisterResource(ID3D12Resource* resource, D3D12_RESOURCE_STATES state)
	{
		if (resource != nullptr)
		{
			std::lock_guard<std::mutex> lock(ms_GlobalRegisterMutex);

			auto iter = ms_GlobalResourceDatas.find(resource);

			if (iter == ms_GlobalResourceDatas.end())
			{
				ms_GlobalResourceDatas[resource].RefCount = 0;
				ms_GlobalResourceDatas[resource].ResourceState.SetResourceState(D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES, state);
			}

			ms_GlobalResourceDatas[resource].RefCount++;
		}
	}
	void ResourceDataMap::DeRegisterResource(ID3D12Resource* resource)
	{
		if (resource != nullptr)
		{
			std::lock_guard<std::mutex> lock(ms_GlobalRegisterMutex);


			
			ms_GlobalResourceDatas[resource].RefCount--;
			if (ms_GlobalResourceDatas[resource].RefCount <= 0)
			{
				ms_GlobalResourceDatas.erase(resource);
			}
		}
	}
	const _ResourceData* ResourceDataMap::GetData(ID3D12Resource* resource)
	{
		assert(ms_IsDataLocked && "Not DataLock While Call ResourceDataMap::GetData())");

		auto iter = ms_GlobalResourceDatas.find(resource);
		if (iter == ms_GlobalResourceDatas.end())
			return nullptr;

		return &ms_GlobalResourceDatas[resource];
	}
	void ResourceDataMap::ResourceBarrier(const D3D12_RESOURCE_BARRIER& barrier)
	{
		if (barrier.Type == D3D12_RESOURCE_BARRIER_TYPE_TRANSITION)
		{
			const auto& transition_barrier = barrier.Transition;
			auto iter = m_FinalResourceState.find(transition_barrier.pResource);




			if (iter != m_FinalResourceState.end())
			{
				auto& resourceState = iter->second;

				// all subresource ���ٰ� ResourceDataMap�� ����������ʴٸ�
				if (transition_barrier.Subresource == D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES &&
					!resourceState.subResourceDataMap.empty())
				{
					for (auto& subresourceState : resourceState.subResourceDataMap)
					{
						if (transition_barrier.StateAfter != subresourceState.second)
						{
							D3D12_RESOURCE_BARRIER new_barrier = barrier;
							new_barrier.Transition.Subresource = subresourceState.first;
							new_barrier.Transition.StateBefore = subresourceState.second;

							m_ResourceBarriers.push_back(new_barrier);
						}
					}
				}
				else
				{
					// ���� ���¸� �ҷ��´�.
					auto finalState = resourceState.GetResourceState(transition_barrier.Subresource);

					// �������¿� ���� ���°� Ʋ���ٸ�
					if (transition_barrier.StateAfter != finalState)
					{
						D3D12_RESOURCE_BARRIER newBarrier = barrier;
						newBarrier.Transition.StateBefore = finalState;
						m_ResourceBarriers.push_back(newBarrier);
					}
				}
			}
			else
			{

				m_PendingResourceBarriers.push_back(barrier);
			}

			m_FinalResourceState[transition_barrier.pResource].SetResourceState(
				transition_barrier.Subresource,
				transition_barrier.StateAfter);
		}
		else
		{
			m_ResourceBarriers.push_back(barrier);
		}
	}
}