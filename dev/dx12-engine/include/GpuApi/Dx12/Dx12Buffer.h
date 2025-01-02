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

	struct Dx12BufferDataDesc
	{
		uint64_t    heapOffset  { 0 };
		const void* srcDataPtr  { nullptr };
		size_t      elementSize { 0 };
		size_t      bufferSize  { 0 };
	};

	class Dx12VertexBuffer
	{
	public:

		void CreateVertexBuffer(
			ID3D12Device* device,
			ID3D12Heap* heap,
			Dx12BufferDataDesc& dataDesc);

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
			Dx12BufferDataDesc& dataDesc);

		size_t GetIndexCount() const;

		D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
		ID3D12Resource* GetIndexBuffer() const;

	private:

		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> indexBuffer;

		size_t indexCount{};
	};

	// Committed resource creation procedures

	/*
	
	template<typename T>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedBufferResource(
			ID3D12Device* device, const std::vector<T>& data)
		{
			size_t typeSize = sizeof(T);
			size_t bufferSize = typeSize * data.size();

			return CreateCommittedBufferResource(device, data.data(), typeSize, bufferSize);
		}

		template <typename VertexType, typename IndexType>
		void CreateMesh(
			const std::vector<VertexType>& vertexBuffer,
			const std::vector<IndexType>& indexBuffer,
			D3D_PRIMITIVE_TOPOLOGY meshTopology)
		{
			bool indexed{ true };

			// TODO
		}

		template <typename VertexType>
		void CreateMesh(
			const std::vector<VertexType>& vertexBuffer,
			D3D_PRIMITIVE_TOPOLOGY meshTopology)
		{
			bool indexed{ false };

			// TODO
		}



		ComPtr<ID3D12Resource> Dx12ResourceManager::CreateCommittedBufferResource(
		ID3D12Device* device,
		void* src, size_t typeSize, size_t bufferSize)
	{
		D3D12_HEAP_PROPERTIES vbHeapProps{};
		vbHeapProps.Type = D3D12_HEAP_TYPE_UPLOAD;
		vbHeapProps.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		vbHeapProps.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		vbHeapProps.CreationNodeMask = 0;
		vbHeapProps.VisibleNodeMask = 0;

		// D3D12_HEAP_FLAGS vbHeapFlags = D3D12_HEAP_FLAG_ALLOW_ONLY_BUFFERS;
		D3D12_HEAP_FLAGS vbHeapFlags = D3D12_HEAP_FLAG_NONE;

		D3D12_RESOURCE_DESC vbResourceDesc{};
		vbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		vbResourceDesc.Alignment = 0; // Must be 64KB for Buffers; 0 is an alias for 64KB
		vbResourceDesc.Width = static_cast<UINT>(bufferSize);
		vbResourceDesc.Height = 1;
		vbResourceDesc.DepthOrArraySize = 1;
		vbResourceDesc.MipLevels = 1;
		vbResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		vbResourceDesc.SampleDesc.Count = 1;
		vbResourceDesc.SampleDesc.Quality = 0;
		vbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		vbResourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

		ComPtr<ID3D12Resource> buffer;
		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommittedResource(
				&vbHeapProps,
				vbHeapFlags,
				&vbResourceDesc,
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(buffer.GetAddressOf())),
			"Failed to create a Vertex Buffer!");

		// Copy the data to the vertex buffer

		UINT8* dst{ nullptr };
		D3D12_RANGE readRange{ 0, 0 };

		DX12_THROW_IF_NOT_SUCCESS(
			buffer->Map(
				0, &readRange, reinterpret_cast<void**>(&dst)),
			"Failed to map the Vertex Buffer!");

		memcpy(dst, src, bufferSize);
		buffer->Unmap(0, nullptr);

		return buffer;
	}

	
	*/
}