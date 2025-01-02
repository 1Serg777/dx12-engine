#include "GpuApi/Dx12/Dx12CommandManager.h"

#include "Core/Error.h"

#include <cassert>

using namespace Microsoft::WRL;

namespace dxe
{
	void Dx12CommandManager::CreateCommandQueues(ID3D12Device* device)
	{
		this->directQueue = CreateDirectQueue(device);
		this->computeQueue = CreateComputeQueue(device);
		this->copyQueue = CreateCopyQueue(device);
	}
	void Dx12CommandManager::DestroyCommandQueues()
	{
		this->directQueue->FlushQueue();
		this->directQueue.reset();

		this->computeQueue->FlushQueue();
		this->computeQueue.reset();

		this->copyQueue->FlushQueue();
		this->copyQueue.reset();
	}

	void Dx12CommandManager::CreateCommandContexts(ID3D12Device* device, uint32_t commandContextSlotCount)
	{
		CreateGraphicsCommandContexts(device, commandContextSlotCount);
	}
	void Dx12CommandManager::DestroyCommandContexts()
	{
		DestroyGraphicsCommandContexts();
		// [TODO] Destroy Compute and Copy command contexts?
	}

	Dx12DirectQueue* Dx12CommandManager::GetDirectQueue() const
	{
		return directQueue.get();
	}
	Dx12ComputeQueue* Dx12CommandManager::GetComputeQueue() const
	{
		return computeQueue.get();
	}
	Dx12CopyQueue* Dx12CommandManager::GetCopyQueue() const
	{
		return copyQueue.get();
	}

	const Dx12GraphicsCommandContext* Dx12CommandManager::GetGraphicsCommandContext(uint32_t commandContextSlotId) const
	{
		bool check = commandContextSlotId >= 0 && commandContextSlotId < commandContextSlotCount;
		assert(check && "Invalid Slot ID provided!");

		// auto retVal = &graphicsCommandContexts[commandContextSlotId];
		// return retVal;

		return &graphicsCommandContexts[commandContextSlotId];
	}

	std::shared_ptr<Dx12DirectQueue> Dx12CommandManager::CreateDirectQueue(ID3D12Device* device)
	{
		std::shared_ptr<Dx12DirectQueue> directQueue = std::make_shared<Dx12DirectQueue>();
		directQueue->InitializeCommandQueue(device);
		return directQueue;
	}
	std::shared_ptr<Dx12ComputeQueue> Dx12CommandManager::CreateComputeQueue(ID3D12Device* device)
	{
		std::shared_ptr<Dx12ComputeQueue> computeQueue = std::make_shared<Dx12ComputeQueue>();
		computeQueue->InitializeCommandQueue(device);
		return computeQueue;
	}
	std::shared_ptr<Dx12CopyQueue> Dx12CommandManager::CreateCopyQueue(ID3D12Device* device)
	{
		std::shared_ptr<Dx12CopyQueue> copyQueue = std::make_shared<Dx12CopyQueue>();
		copyQueue->InitializeCommandQueue(device);
		return copyQueue;
	}
	
	
	void Dx12CommandManager::CreateGraphicsCommandContexts(ID3D12Device* device, uint32_t commandContextSlotCount)
	{
		this->commandContextSlotCount = commandContextSlotCount;
		graphicsCommandContexts.resize(commandContextSlotCount);
		for (uint32_t slot = 0; slot < commandContextSlotCount; slot++)
		{
			graphicsCommandContexts[slot].commandAllocator =
				CreateCommandAllocator(
					device, D3D12_COMMAND_LIST_TYPE_DIRECT);

			graphicsCommandContexts[slot].graphicsCommandList =
				CreateGraphicsCommandList(
					device, D3D12_COMMAND_LIST_TYPE_DIRECT, graphicsCommandContexts[slot].commandAllocator.Get());
		}
	}
	void Dx12CommandManager::DestroyGraphicsCommandContexts()
	{
		directQueue->FlushQueue();
		graphicsCommandContexts.clear();
	}

	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> Dx12CommandManager::CreateCommandAllocator(
		ID3D12Device* device,
		D3D12_COMMAND_LIST_TYPE allocatorType) const
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandAllocator(
				allocatorType,
				IID_PPV_ARGS(commandAllocator.GetAddressOf())),
			"Failed to create a Command Allocator!");

		return commandAllocator;
	}
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> Dx12CommandManager::CreateGraphicsCommandList(
		ID3D12Device* device,
		D3D12_COMMAND_LIST_TYPE cmdListType, ID3D12CommandAllocator* commandAllocator) const
	{
		ComPtr<ID3D12GraphicsCommandList7> commandList;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandList(
				0, cmdListType,
				commandAllocator, nullptr,
				IID_PPV_ARGS(commandList.GetAddressOf())),
			"Failed to create a Graphics Command List!");

		commandList->Close();

		return commandList;
	}
}