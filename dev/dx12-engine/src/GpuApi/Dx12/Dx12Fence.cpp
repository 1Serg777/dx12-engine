#include "GpuApi/Dx12/Dx12Fence.h"

#include "Core/Error.h"

namespace dxe
{
	void Dx12Fence::Initialize(ID3D12Device* device)
	{
		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateFence(
				0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())),
			"Failed to create a Fence object!");

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		WINAPI_THROW_IF_NULL(fenceEvent, "Failed to create a Fence Event!");

		// fenceValue++;
		// fenceValue = 1;
	}

	void Dx12Fence::SignalOnGpu(ID3D12CommandQueue* commandQueue)
	{
		fenceValue++;
		DX12_THROW_IF_NOT_SUCCESS(
			commandQueue->Signal(fence.Get(), fenceValue),
			"Failed to signal a fence value!");
	}
	void Dx12Fence::SignalOnCpu()
	{
		fenceValue++;
		fence->Signal(fenceValue);
	}

	void Dx12Fence::WaitForFenceEvent()
	{
		if (fence->GetCompletedValue() < fenceValue)
		{
			DX12_THROW_IF_NOT_SUCCESS(
				fence->SetEventOnCompletion(fenceValue, fenceEvent),
				"Couldn't set the fence event!");

			WaitForSingleObject(fenceEvent, INFINITE);
		}
	}

	void Dx12Fence::SignalOnGpuAndWaitForFenceEvent(ID3D12CommandQueue* commandQueue)
	{
		SignalOnGpu(commandQueue);
		WaitForFenceEvent();
	}
}