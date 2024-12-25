#pragma once

#include <d3d12.h>

#include <wrl/client.h>

#include <cstdint>

namespace dxe
{
	class Dx12Fence
	{
	public:

		Dx12Fence() = default;
		~Dx12Fence() = default;

		void Initialize(ID3D12Device* device);

		void SignalOnGpu(ID3D12CommandQueue* commandQueue);
		void SignalOnCpu();

		void WaitForFenceEvent();

		void SignalOnGpuAndWaitForFenceEvent(ID3D12CommandQueue* commandQueue);

	private:

		Microsoft::WRL::ComPtr<ID3D12Fence> fence;
		HANDLE fenceEvent{ NULL };
		uint64_t fenceValue{ 0 };
	};
}