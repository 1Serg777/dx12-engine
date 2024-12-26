#pragma once

#include <d3d12.h>

namespace dxe
{
	class Dx12Barrier
	{
	public:

		static D3D12_RESOURCE_BARRIER CreateTransitionBarrier(
			ID3D12Resource* pResource,
			D3D12_RESOURCE_STATES stateBefore,
			D3D12_RESOURCE_STATES stateAfter,
			UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

		static D3D12_RESOURCE_BARRIER CreateRenderTargetToPresentTransitionBarrier(
			ID3D12Resource* pResource,
			UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);

		static D3D12_RESOURCE_BARRIER CreatePresentToRenderTargetTransitionBarrier(
			ID3D12Resource* pResource,
			UINT subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES,
			D3D12_RESOURCE_BARRIER_FLAGS flags = D3D12_RESOURCE_BARRIER_FLAG_NONE);
	};
}