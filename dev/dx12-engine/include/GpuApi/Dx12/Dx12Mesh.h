#pragma once

#include "GpuApi/Dx12/Dx12Buffer.h"

#include "Renderer/Vertex.h"

#include <d3d12.h>

#include <wrl/client.h>

#include <memory>
#include <vector>

namespace dxe
{
	class Dx12Mesh
	{
	public:

		template<typename VertexType, typename IndexType>
		void CreateMesh(
			ID3D12Device* device,
			const std::vector<VertexType>& vertices,
			const std::vector<IndexType>& indices)
		{
			size_t vertexSize = VertexType::stride;
			size_t vertexBufferSize = vertexSize * vertices.size();

			size_t indexSize = sizeof(IndexType);
			size_t indexBufferSize = indexSize * indices.size();

			InitializeHeap(device, vertexSize, indexSize);

			CreateVertexBuffer(device, vertices.data(), vertexSize, vertexBufferSize);
			CreateIndexBuffer(device, indices.data(), indexSize, indexBufferSize);
		}

		template<typename VertexType>
		void CreateMesh(
			ID3D12Device* device,
			const std::vector<VertexType>& vertices)
		{
			size_t vertexSize = VertexType::stride;
			size_t vertexBufferSize = vertexSize * vertices.size();

			InitializeHeap(device, vertexSize, 0);

			CreateVertexBuffer(device, vertices.data(), vertexSize, vertexBufferSize);
		}

		Dx12VertexBuffer* GetVertexBuffer() const;
		Dx12IndexBuffer* GetIndexBuffer() const;

	private:

		void InitializeHeap(
			ID3D12Device* device,
			size_t requestedVbSize,
			size_t requestedIbSize);

		void CreateVertexBuffer(ID3D12Device* device, const void* vertSrcData, size_t vertexSize, size_t vertexBufferSize);
		void CreateIndexBuffer(ID3D12Device* device, const void* indexSrcData, size_t indexSize, size_t indexDataSize);

		std::unique_ptr<Dx12VertexBuffer> vertexBuffer;
		std::unique_ptr<Dx12IndexBuffer> indexBuffer;

		Microsoft::WRL::ComPtr<ID3D12Heap> heap;

		size_t heapSize{ 0 };

		size_t vbOffset{ 0 };
		size_t ibOffset{ 0 };
	};
}