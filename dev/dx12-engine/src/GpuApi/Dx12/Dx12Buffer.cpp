#include "GpuApi/Dx12/Dx12Buffer.h"

#include "Core/Error.h"
#include "Core/Logger.h"

namespace dxe
{
	void Dx12VertexBuffer::CreateVertexBuffer(
		ID3D12Device* device,
		ID3D12Heap* heap,
		uint64_t heapOffset,
		const void* srcDataPtr,
		size_t vertexSize,
		size_t vertexBufferSize)
	{
		this->vertexCount = vertexBufferSize / vertexSize;

		D3D12_RESOURCE_DESC vbResourceDesc{};
		vbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbResourceDesc.Alignment = 0; // Must be 64KB for Buffers; 0 is an alias for 64KB
		vbResourceDesc.Width = vertexBufferSize;
		vbResourceDesc.Height = 1;
		vbResourceDesc.DepthOrArraySize = 1;
		vbResourceDesc.MipLevels = 1;
		vbResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		vbResourceDesc.SampleDesc.Count = 1;
		vbResourceDesc.SampleDesc.Quality = 0;
		vbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vbResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreatePlacedResource(
				heap, heapOffset,
				&vbResourceDesc,
				initialResourceState,
				nullptr,
				IID_PPV_ARGS(vertexBuffer.ReleaseAndGetAddressOf())),
			"Failed to create a Vertex Buffer Placed Resource!");

		// Copy the data to the vertex buffer

		UINT8* vertexBufferDataPtr{ nullptr };
		D3D12_RANGE readRange{ 0, 0 };

		DX12_THROW_IF_NOT_SUCCESS(
			vertexBuffer->Map(
				0, &readRange, reinterpret_cast<void**>(&vertexBufferDataPtr)),
			"Failed to map the Vertex Buffer!");

		memcpy(vertexBufferDataPtr, srcDataPtr, vertexBufferSize);
		vertexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view

		vertexBufferView.BufferLocation = vertexBuffer->GetGPUVirtualAddress();
		vertexBufferView.StrideInBytes = static_cast<UINT>(vertexSize);
		vertexBufferView.SizeInBytes = static_cast<UINT>(vertexBufferSize);
	}

	size_t Dx12VertexBuffer::GetVertexCount() const
	{
		return this->vertexCount;
	}

	D3D12_VERTEX_BUFFER_VIEW Dx12VertexBuffer::GetVertexBufferView() const
	{
		return vertexBufferView;
	}
	ID3D12Resource* Dx12VertexBuffer::GetVertexBuffer() const
	{
		return vertexBuffer.Get();
	}

	void Dx12IndexBuffer::CreateIndexBuffer(
		ID3D12Device* device,
		ID3D12Heap* heap,
		uint64_t heapOffset,
		const void* srcDataPtr,
		size_t indexSize,
		size_t indexBufferSize)
	{
		this->indexCount = indexBufferSize / indexSize;

		D3D12_RESOURCE_DESC vbResourceDesc{};
		vbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbResourceDesc.Alignment = 0; // Must be 64KB for Buffers; 0 is an alias for 64KB
		vbResourceDesc.Width = indexBufferSize;
		vbResourceDesc.Height = 1;
		vbResourceDesc.DepthOrArraySize = 1;
		vbResourceDesc.MipLevels = 1;
		vbResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		vbResourceDesc.SampleDesc.Count = 1;
		vbResourceDesc.SampleDesc.Quality = 0;
		vbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vbResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		// D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_INDEX_BUFFER;
		D3D12_RESOURCE_STATES initialResourceState = D3D12_RESOURCE_STATE_GENERIC_READ;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreatePlacedResource(
				heap, heapOffset,
				&vbResourceDesc,
				initialResourceState,
				nullptr,
				IID_PPV_ARGS(indexBuffer.ReleaseAndGetAddressOf())),
			"Failed to create a Vertex Buffer Placed Resource!");

		// Copy the data to the vertex buffer

		UINT8* vertexBufferDataPtr{ nullptr };
		D3D12_RANGE readRange{ 0, 0 };

		DX12_THROW_IF_NOT_SUCCESS(
			indexBuffer->Map(
				0, &readRange, reinterpret_cast<void**>(&vertexBufferDataPtr)),
			"Failed to map the Vertex Buffer!");

		memcpy(vertexBufferDataPtr, srcDataPtr, indexBufferSize);
		indexBuffer->Unmap(0, nullptr);

		// Initialize the vertex buffer view

		indexBufferView.BufferLocation = indexBuffer->GetGPUVirtualAddress();
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
		indexBufferView.SizeInBytes = static_cast<UINT>(indexBufferSize);
	}

	size_t Dx12IndexBuffer::GetIndexCount() const
	{
		return this->indexCount;
	}

	D3D12_INDEX_BUFFER_VIEW Dx12IndexBuffer::GetIndexBufferView() const
	{
		return indexBufferView;
	}
	ID3D12Resource* Dx12IndexBuffer::GetIndexBuffer() const
	{
		return indexBuffer.Get();
	}
}