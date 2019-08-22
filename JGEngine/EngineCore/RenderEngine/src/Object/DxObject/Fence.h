#pragma once
#include "DxObject.h"

namespace RE
{
	class Fence : public DxObject
	{
	private:
		ComPtr<ID3D12Fence>  m_D3D_Fence;
		std::mutex m_EventMutex;
		std::atomic_uint64_t m_FenceValue;
		HANDLE   m_FenceEvent;
	public:
		Fence();
		~Fence();
		// Fence �� ��������
		uint64_t GetValue() const {
			return m_FenceValue;
		}
		// Fence �� ����
		void IncreaseValue() {
			m_FenceValue.fetch_add(1);
		}
		// Fence ������ ����ȭ 
		void WaitForFenceValue(uint64_t value, std::chrono::milliseconds duration = std::chrono::milliseconds::max());

		// ID3D12Fence* ��������
		ID3D12Fence* Get() const {
			return m_D3D_Fence.Get();
		}
	};
}