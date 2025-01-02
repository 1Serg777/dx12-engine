#include "GpuApi/Dx12/Dx12SwapChain.h"

#include "Core/Error.h"

#include "Window/WindowWin32.h"

using namespace Microsoft::WRL;

namespace dxe
{
	Dx12SwapChain::Dx12SwapChain(
		uint32_t frameBufferCount,
		DXGI_FORMAT rtvFormat,
		DXGI_SWAP_EFFECT swapChainEffect)
		: frameBufferCount(frameBufferCount),
		rtvFormat(rtvFormat),
		swapChainEffect(swapChainEffect)
	{
		swapChainBuffers.resize(frameBufferCount);
	}

	Dx12SwapChain::~Dx12SwapChain()
	{
		rtvHeap.reset();
		swapChainBuffers.clear();
		swapChain.Reset();
	}

	void Dx12SwapChain::CreateSwapChain(
		ID3D12Device* device,
		IDXGIFactory2* factory,
		ID3D12CommandQueue* commandQueue,
		WindowWin32* window)
	{
		CreateRtvHeap(device);
		CreateSwapChainImpl(factory, commandQueue, window);
		frameBufferIndex = this->swapChain->GetCurrentBackBufferIndex();
		CreateFrameResources(device);
	}
	void Dx12SwapChain::ResizeSwapChain(uint32_t width, uint32_t height)
	{
		// TODO
	}

	void Dx12SwapChain::Present()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			swapChain->Present(1, 0),
			"Failed to present a swap chain image!");
		
		frameBufferIndex = swapChain->GetCurrentBackBufferIndex();
	}

	uint32_t Dx12SwapChain::GetFrameBufferCount() const
	{
		return frameBufferCount;
	}
	uint32_t Dx12SwapChain::GetCurrentBackBufferIndex() const
	{
		return frameBufferIndex;
	}

	ID3D12Resource* Dx12SwapChain::GetCurrentBackBufferResource() const
	{
		return swapChainBuffers[frameBufferIndex].Get();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE Dx12SwapChain::GetCurrentBackBufferRTV() const
	{
		return rtvHeap->GetDescriptorHandle(frameBufferIndex);;
	}

	void Dx12SwapChain::CreateRtvHeap(ID3D12Device* device)
	{
		rtvHeap = std::make_shared<Dx12DescriptorHeap>(frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		rtvHeap->Initialize(device);
	}

	void Dx12SwapChain::CreateSwapChainImpl(IDXGIFactory2* factory, ID3D12CommandQueue* commandQueue, WindowWin32* window)
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = frameBufferCount;
		swapChainDesc.Width = window->GetWindowWidth();
		swapChainDesc.Height = window->GetWindowHeight();
		swapChainDesc.Format = rtvFormat;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = swapChainEffect;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapChain;	
		HRESULT hr = factory->CreateSwapChainForHwnd(
			commandQueue,
			window->GetWindowHandle(),
			&swapChainDesc,
			nullptr,
			nullptr,
			swapChain.GetAddressOf()
		);
		DX12_THROW_IF_NOT_SUCCESS(
			hr,
			"Failed to create a swap chain!");

		DX12_THROW_IF_NOT_SUCCESS(
			swapChain.As(&this->swapChain),
			"Couldn't cast IDXGISwapChain1 to IDXGISwapChain4!");

		// Don't support fullscreen transitions for now.
		DX12_THROW_IF_NOT_SUCCESS(
			factory->MakeWindowAssociation(window->GetWindowHandle(), DXGI_MWA_NO_ALT_ENTER),
			"Failed to make a Win32-SwapChain association!");
	}

	void Dx12SwapChain::CreateFrameResources(ID3D12Device* device)
	{
		for (uint32_t frameIdx{ 0 }; frameIdx < frameBufferCount; frameIdx++)
		{
			DX12_THROW_IF_NOT_SUCCESS(
				swapChain->GetBuffer(
					frameIdx, IID_PPV_ARGS(swapChainBuffers[frameIdx].ReleaseAndGetAddressOf())),
				"Failed to acquire a Swap Chain Frame Buffer!");

			device->CreateRenderTargetView(
				swapChainBuffers[frameIdx].Get(),
				nullptr,
				rtvHeap->GetDescriptorHandle(frameIdx));
		}
	}
}