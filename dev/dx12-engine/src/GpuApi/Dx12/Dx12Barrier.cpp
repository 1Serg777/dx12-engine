#include "GpuApi/Dx12/Dx12Barrier.h"

namespace dxe
{
    D3D12_RESOURCE_BARRIER Dx12Barrier::CreateTransitionBarrier(
        ID3D12Resource* pResource,
        D3D12_RESOURCE_STATES stateBefore,
        D3D12_RESOURCE_STATES stateAfter,
        UINT subresource,
        D3D12_RESOURCE_BARRIER_FLAGS flags)
    {
        D3D12_RESOURCE_BARRIER resourceBarrier{};
        resourceBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        resourceBarrier.Transition.pResource = pResource;
        resourceBarrier.Transition.StateAfter = stateAfter;
        resourceBarrier.Transition.StateBefore = stateBefore;
        resourceBarrier.Transition.Subresource = subresource;
        resourceBarrier.Flags = flags;
        return resourceBarrier;
    }

    D3D12_RESOURCE_BARRIER Dx12Barrier::CreateRenderTargetToPresentTransitionBarrier(
        ID3D12Resource* pResource,
        UINT subresource,
        D3D12_RESOURCE_BARRIER_FLAGS flags)
    {
        return CreateTransitionBarrier(
            pResource,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            D3D12_RESOURCE_STATE_PRESENT,
            subresource,
            flags);
    }

    D3D12_RESOURCE_BARRIER Dx12Barrier::CreatePresentToRenderTargetTransitionBarrier(
        ID3D12Resource* pResource,
        UINT subresource,
        D3D12_RESOURCE_BARRIER_FLAGS flags)
    {
        return CreateTransitionBarrier(
            pResource,
            D3D12_RESOURCE_STATE_PRESENT,
            D3D12_RESOURCE_STATE_RENDER_TARGET,
            subresource,
            flags);
    }
}
