#pragma once

#include <d3d12.h>

#include <wrl/client.h>

#include <cstdint>

namespace dxe
{
	class Dx12Buffer
	{
	public:

	private:

	};

	class Dx12VertexBuffer
	{
	public:

		void CreateVertexBuffer(
			ID3D12Device* device,
			ID3D12Heap* heap,
			uint64_t heapOffset,
			const void* srcDataPtr,
			size_t vertexSize,
			size_t vertexBufferSize);

		size_t GetVertexCount() const;

		D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;
		ID3D12Resource* GetVertexBuffer() const;

	private:

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;

		size_t vertexCount{};
	};

	class Dx12IndexBuffer
	{
	public:

		void CreateIndexBuffer(
			ID3D12Device* device,
			ID3D12Heap* heap,
			uint64_t heapOffset,
			const void* srcDataPtr,
			size_t indexSize,
			size_t indexBufferSize);

		size_t GetIndexCount() const;

		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
		ID3D12Resource* GetIndexBuffer() const;

	private:

		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

		size_t indexCount{};
	};
}