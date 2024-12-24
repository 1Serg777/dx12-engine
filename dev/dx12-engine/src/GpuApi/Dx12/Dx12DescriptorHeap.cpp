#include "GpuApi/Dx12/Dx12DescriptorHeap.h"

#include "Core/Error.h"

namespace dxe
{
	Dx12DescriptorHeap::Dx12DescriptorHeap(
		size_t descriptorCount,
		D3D12_DESCRIPTOR_HEAP_TYPE descriptorHeapType,
		D3D12_DESCRIPTOR_HEAP_FLAGS descriptorHeapFlags)
		: descriptorCount(descriptorCount),
		descriptorHeapType(descriptorHeapType),
		descriptorHeapFlags(descriptorHeapFlags)
	{
	}

	void Dx12DescriptorHeap::Initialize(ID3D12Device* device)
	{
		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
		rtvHeapDesc.NumDescriptors = descriptorCount;
		rtvHeapDesc.Type = descriptorHeapType;
		rtvHeapDesc.Flags = descriptorHeapFlags;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateDescriptorHeap(
				&rtvHeapDesc,
				IID_PPV_ARGS(descriptorHeap.ReleaseAndGetAddressOf())),
			"Failed to create a Descriptor Heap!");

		descriptorSize = device->GetDescriptorHandleIncrementSize(descriptorHeapType);
		heapStartHandle = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	}

	D3D12_CPU_DESCRIPTOR_HANDLE Dx12DescriptorHeap::GetDescriptorHandle(size_t descriptorIndex)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE heapHandle = heapStartHandle;
		heapHandle.ptr += descriptorIndex * descriptorSize;
		return heapHandle;
	}
}