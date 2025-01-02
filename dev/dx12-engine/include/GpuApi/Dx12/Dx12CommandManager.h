#pragma once

#include "GpuApi/Dx12/Dx12Queue.h"

#include "Core/Utility.h"

#include <d3d12.h>

#include <cstdint>
#include <memory>
#include <sstream>
#include <vector>

#include <wrl/client.h>

namespace dxe
{
	struct Dx12GraphicsCommandContext
	{
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> graphicsCommandList;
	};

	class Dx12CommandManager
	{
	public:

		Dx12CommandManager() = default;
		~Dx12CommandManager() = default;

		CLASS_NO_COPY(Dx12CommandManager);
		CLASS_NO_MOVE(Dx12CommandManager);

		void CreateCommandQueues(ID3D12Device* device);
		void DestroyCommandQueues();

		void CreateCommandContexts(ID3D12Device* device, uint32_t commandContextSlotCount);
		void DestroyCommandContexts();

		Dx12DirectQueue* GetDirectQueue() const;
		Dx12ComputeQueue* GetComputeQueue() const;
		Dx12CopyQueue* GetCopyQueue() const;

		const Dx12GraphicsCommandContext* GetGraphicsCommandContext(uint32_t commandContextSlotId) const;

	private:

		std::shared_ptr<Dx12DirectQueue> CreateDirectQueue(ID3D12Device* device);
		std::shared_ptr<Dx12ComputeQueue> CreateComputeQueue(ID3D12Device* device);
		std::shared_ptr<Dx12CopyQueue> CreateCopyQueue(ID3D12Device* device);

		void CreateGraphicsCommandContexts(ID3D12Device* device, uint32_t commandContextSlotCount);
		void DestroyGraphicsCommandContexts();

		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CreateCommandAllocator(
			ID3D12Device* device,
			D3D12_COMMAND_LIST_TYPE allocatorType) const;

		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CreateGraphicsCommandList(
			ID3D12Device* device,
			D3D12_COMMAND_LIST_TYPE cmdListType, ID3D12CommandAllocator* commandAllocator) const;

		uint32_t commandContextSlotCount{};

		std::vector<Dx12GraphicsCommandContext> graphicsCommandContexts;

		std::shared_ptr<Dx12DirectQueue> directQueue;
		std::shared_ptr<Dx12ComputeQueue> computeQueue;
		std::shared_ptr<Dx12CopyQueue> copyQueue;	
	};
}