#pragma once

#include <d3d12.h>

#include <wrl/client.h>

namespace dxe
{
	class Dx12DescriptorHeap
	{
	public:

		Dx12DescriptorHeap(
			size_t descriptorCount,
			D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType,
			D3D12_DESCRIPTOR_HEAP_FLAGS descriptorHeapFlags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE);

		void Initialize(ID3D12Device* device);

		D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorHandle(size_t descriptorIndex);

	private:

		Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> descriptorHeap;

		D3D12_CPU_DESCRIPTOR_HANDLE heapStartHandle{};

		size_t descriptorCount{ 0 };
		size_t descriptorSize{ 0 };

		D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType{};
		D3D12_DESCRIPTOR_HEAP_FLAGS descriptorHeapFlags{};
	};
}