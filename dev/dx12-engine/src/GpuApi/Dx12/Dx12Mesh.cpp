#include "GpuApi/Dx12/Dx12Mesh.h"

#include "Core/Error.h"

#include <cmath>

namespace dxe
{
	// Sizes below are specified in bytes

	constexpr size_t bufferAllignment = 65536;
	constexpr size_t bufferAllignmentMask = bufferAllignment - 1;

	Dx12VertexBuffer* Dx12Mesh::GetVertexBuffer() const
	{
		return vertexBuffer.get();
	}
	Dx12IndexBuffer* Dx12Mesh::GetIndexBuffer() const
	{
		return indexBuffer.get();
	}

	void Dx12Mesh::InitializeHeap(
		ID3D12Device* device,
		size_t requestedVbSize,
		size_t requestedIbSize)
	{
		// Determine the sizes of the vertex and index buffers in terms of
		// multiples of the buffer allignment requirement (64 KB).

		size_t vbAllocSizeMult = static_cast<uint32_t>(
			std::ceil(
				static_cast<double>(requestedVbSize) / bufferAllignment
			)
		);
		size_t ibAllocSizeMult = static_cast<uint32_t>(
			std::ceil(
				static_cast<double>(requestedIbSize) / bufferAllignment
			)
		);
		
		this->vbOffset = 0;
		this->ibOffset = vbAllocSizeMult * bufferAllignment;

		this->heapSize = (vbAllocSizeMult + ibAllocSizeMult) * bufferAllignment;

		D3D12_HEAP_PROPERTIES heapProps{};
		// CPU-visible UPLOAD type for now,
		// but should be configured based on whether the mesh is dynamic or not.
		heapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		heapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProps.CreationNodeMask = 0;
		heapProps.VisibleNodeMask = 0;

		D3D12_HEAP_DESC heapDesc{};
		heapDesc.Alignment = 0; // Which is an alias for 64 KB
		// Automatically assigned for implicit heaps,
		// but here we only care about buffers.
		// 
		// heapDesc.Flags = D3D12_HEAP_FLAG_NONE;
		heapDesc.Flags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
		heapDesc.Properties = heapProps;
		heapDesc.SizeInBytes = this->heapSize;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateHeap(&heapDesc, IID_PPV_ARGS(heap.ReleaseAndGetAddressOf())),
			"Failed to create a Heap for Mesh Data!");
	}

	void Dx12Mesh::CreateVertexBuffer(ID3D12Device* device, const void* vertSrcData, size_t vertexSize, size_t vertexBufferSize)
	{
		vertexBuffer = std::make_unique<Dx12VertexBuffer>();
		vertexBuffer->CreateVertexBuffer(
			device, heap.Get(), vbOffset,
			vertSrcData, vertexSize, vertexBufferSize);
	}
	void Dx12Mesh::CreateIndexBuffer(ID3D12Device* device, const void* indexSrcData, size_t indexSize, size_t indexBufferSize)
	{
		indexBuffer = std::make_unique<Dx12IndexBuffer>();
		indexBuffer->CreateIndexBuffer(
			device, heap.Get(), ibOffset,
			indexSrcData, indexSize, indexBufferSize);
	}
}