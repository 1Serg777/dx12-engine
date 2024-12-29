#include "GpuApi/Dx12/Dx12Queue.h"

#include "Core/Error.h"

namespace dxe
{
	void Dx12Queue::SignalFence(Dx12Fence* fence)
	{
		fence->SignalOnGpu(commandQueue.Get());
	}
	void Dx12Queue::FlushQueue()
	{
		queueFence->SignalOnGpu(commandQueue.Get());
		queueFence->WaitForFenceEvent();
	}

	ID3D12CommandQueue* Dx12Queue::GetCommandQueue() const
	{
		return commandQueue.Get();
	}

	void Dx12Queue::CreateCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE queueType)
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = queueType;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandQueue(
				&queueDesc,
				IID_PPV_ARGS(commandQueue.ReleaseAndGetAddressOf())),
			"Failed to create a Command Queue!");
	}
	void Dx12Queue::CreateQueueFence(ID3D12Device* device)
	{
		queueFence = std::make_shared<Dx12Fence>();
		queueFence->Initialize(device);
	}

	void Dx12DirectQueue::InitializeCommandQueue(ID3D12Device* device)
	{
		CreateCommandQueue(device, D3D12_COMMAND_LIST_TYPE_DIRECT);
		CreateQueueFence(device);
	}

	void Dx12ComputeQueue::InitializeCommandQueue(ID3D12Device* device)
	{
		CreateCommandQueue(device, D3D12_COMMAND_LIST_TYPE_COMPUTE);
		CreateQueueFence(device);
	}
}