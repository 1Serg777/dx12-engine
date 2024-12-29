#pragma once

#include "GpuApi/Dx12/Dx12Fence.h"

#include <d3d12.h>

#include <memory>

namespace dxe
{
	class Dx12Queue
	{
	public:

		virtual void InitializeCommandQueue(ID3D12Device* device) = 0;

		void SignalFence(Dx12Fence* fence);
		void FlushQueue();

		ID3D12CommandQueue* GetCommandQueue() const;

	protected:

		void CreateCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE queueType);
		void CreateQueueFence(ID3D12Device* device);

		std::shared_ptr<Dx12Fence> queueFence;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
	};

	class Dx12DirectQueue : public Dx12Queue
	{
	public:

		void InitializeCommandQueue(ID3D12Device* device) override;

	private:

	};

	class Dx12ComputeQueue : public Dx12Queue
	{
	public:

		void InitializeCommandQueue(ID3D12Device* device) override;

	private:

	};
}