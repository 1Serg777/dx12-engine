#pragma once

#include "GpuApi/Dx12/Dx12Fence.h"
#include "GpuApi/Dx12/Dx12PSO.h"
#include "GpuApi/Dx12/Dx12RootSignature.h"

#include <d3d12.h>

#include <cstdint>
#include <memory>

#include <wrl/client.h>

namespace dxe
{
	struct RenderData
	{
		uint32_t meshId{ 0 };
		uint32_t rootSignatureId{ 0 };
		uint32_t graphicsPSOId{ 0 };
	};

	class Dx12Renderer
	{
	public:

		void Initialize();
		void Terminate();

		void Render(const RenderData& renderData) const;

		void SetViewport(uint32_t width, uint32_t height);
		void SetScissors(uint32_t right, uint32_t bottom);

	private:

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};
	};
}