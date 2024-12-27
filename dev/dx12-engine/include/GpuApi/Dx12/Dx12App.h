#pragma once

#include "Core/Application.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include "GpuApi/Dx12/Dx12DescriptorHeap.h"
#include "GpuApi/Dx12/Dx12Fence.h"
#include "GpuApi/Dx12/Dx12PSO.h"
#include "GpuApi/Dx12/Dx12RootSignature.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <cstdint>
#include <memory>
#include <sstream>

#include <wrl/client.h>

namespace dxe
{
	class Dx12App : public Application
	{
	public:

		void Initialize() override;
		void Terminate() override;
		void Run() override;

	private:

		void InitializeWindow();

		void InitializeDx12();

		void Render();

		void CreateFactory(UINT factoryFlags);

#if defined(DEBUG) || defined(_DEBUG)
		void InitializeDebugLayers();
		void InitializeDebugCallbackFunction();
#endif

		void PickAdapter();
		void LogAdapter(std::wstringstream& logStream, UINT adapterIdx, DXGI_ADAPTER_DESC3& adapterDesc);
		void LogAdapterOutputs(std::wstringstream& logStream, Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter);
		void LogOutputDisplayModes(std::wstringstream& logStream, Microsoft::WRL::ComPtr<IDXGIOutput6> output);

		void CreateDevice();

		void CreateCommandObjects();
		void CreateCommandQueue();
		void CreateCommandAllocators();

		void CreateCommandLists();
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> CreateGraphicsCommandList(ID3D12CommandAllocator* commandAllocator);

		void CreateSwapChain();

		void CreateDescriptorHeaps();
		void CreateRTVDescriptorHeap();

		void CreateFrameResources();

		void CreateSynchronizationObjects();

		void LoadAssets();

		void WaitForFrameToFinish();

		void ResizeViewport();
		void ResizeScissors();

		void OnWindowClose(const WindowCloseCallbackData& callbackData);

		static void DebugMessenger(
			D3D12_MESSAGE_CATEGORY Category,
			D3D12_MESSAGE_SEVERITY Severity,
			D3D12_MESSAGE_ID ID,
			LPCSTR pDescription,
			void* pContext);

		std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> swapChainBuffers;

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};

		std::shared_ptr<EventRegistry> eventRegistry;
		std::shared_ptr<WindowWin32> window;

		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;

#if defined(DEBUG) || defined(_DEBUG)
		Microsoft::WRL::ComPtr<ID3D12Debug6> debugLayer;
		Microsoft::WRL::ComPtr<ID3D12InfoQueue1> infoQueue;
#endif

		Microsoft::WRL::ComPtr<IDXGIFactory7> factory;

		Microsoft::WRL::ComPtr<IDXGIAdapter4> adapter;
		Microsoft::WRL::ComPtr<ID3D12Device10> device;

		Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> graphicsCommandList;

		Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain;

		std::shared_ptr<Dx12DescriptorHeap> rtvHeap;
		
		std::shared_ptr<Dx12Fence> frameCompletedFence;

		std::shared_ptr<Dx12RootSignature> rootSignature;
		std::shared_ptr<Dx12GraphicsPSO> graphicsPSO;

		DWORD callbackCookie{ 0 };

		uint32_t frameBufferCount{ 0 };
		uint32_t frameBufferIndex{ 0 };

		bool appIsRunning{ false };
	};
}