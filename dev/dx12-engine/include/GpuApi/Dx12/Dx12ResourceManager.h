#pragma once

#include <d3d12.h>

#include <wrl/client.h>

#include <memory>
#include <vector>

namespace dxe
{
	class Dx12ResourceManager
	{
	public:

		template<typename T>
		Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedBufferResource(
			ID3D12Device* device, const std::vector<T>& data)
		{
			size_t typeSize = sizeof(T);
			size_t bufferSize = typeSize * data.size();

			return CreateCommittedBufferResource(device, data.data(), typeSize, bufferSize);
		}

	private:

		Microsoft::WRL::ComPtr<ID3D12Resource> CreateCommittedBufferResource(
			ID3D12Device* device,
			void* src, size_t typeSize, size_t bufferSize);

	};
}