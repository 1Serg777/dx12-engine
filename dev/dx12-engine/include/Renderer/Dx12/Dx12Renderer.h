#pragma once

#include "GpuApi/Dx12/Dx12PSO.h"
#include "GpuApi/Dx12/Dx12RootSignature.h"

#include <d3d12.h>

#include <memory>

#include <wrl/client.h>

namespace dxe
{
	class Dx12Renderer
	{
	public:

		void Initialize();

	private:

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};

		std::shared_ptr<Dx12RootSignature> rootSignature;
		std::shared_ptr<Dx12GraphicsPSO> graphicsPSO;
	};
}