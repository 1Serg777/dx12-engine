#pragma once

#include <d3d12.h>

#include <wrl/client.h>

namespace dxe
{
	class Dx12RootSignature
	{
	public:

		void CreateRootSignature(ID3D12Device* device);

		ID3D12RootSignature* GetRootSignature() const;

	private:

		Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature;
	};
}