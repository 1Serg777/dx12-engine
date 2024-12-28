#pragma once

#include "GpuApi/Dx12/Dx12DescriptorHeap.h"

#include <d3d12.h>

#include <dxgi1_6.h>

#include <cstdint>
#include <memory>
#include <vector>

#include <wrl/client.h>

namespace dxe
{
	class WindowWin32;

	class Dx12SwapChain
	{
	public:

		Dx12SwapChain(
			uint32_t frameBufferCount,
			DXGI_FORMAT rtvFormat = DXGI_FORMAT_R8G8B8A8_UNORM,
			DXGI_SWAP_EFFECT swapChainMode = DXGI_SWAP_EFFECT_FLIP_DISCARD);

		~Dx12SwapChain();

		void CreateSwapChain(
			ID3D12Device* device,
			IDXGIFactory2* factory,
			ID3D12CommandQueue* commandQueue,
			WindowWin32* window);

		void ResizeSwapChain(uint32_t width, uint32_t height);

		void Present();

		uint32_t GetFrameBufferCount() const;
		uint32_t GetCurrentBackBufferIndex() const;

		ID3D12Resource* GetCurrentBackBufferResource() const;
		D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferRTV() const;

	private:

		void CreateRtvHeap(ID3D12Device* device);

		void CreateSwapChainImpl(
			IDXGIFactory2* factory,
			ID3D12CommandQueue* commandQueue,
			WindowWin32* window);

		void CreateFrameResources(ID3D12Device* device);

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> swapChainBuffers;

		std::shared_ptr<Dx12DescriptorHeap> rtvHeap;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

		DXGI_FORMAT rtvFormat{};
		DXGI_SWAP_EFFECT swapChainEffect{};

		uint32_t frameBufferCount{ 0 };
		uint32_t frameBufferIndex{ 0 };
	};
}