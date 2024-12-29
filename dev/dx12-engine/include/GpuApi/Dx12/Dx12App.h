#pragma once

#include "Core/Application.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include "GpuApi/Dx12/Dx12Device.h"
#include "GpuApi/Dx12/Dx12DescriptorHeap.h"
#include "GpuApi/Dx12/Dx12Fence.h"
#include "GpuApi/Dx12/Dx12Mesh.h"
#include "GpuApi/Dx12/Dx12PSO.h"
#include "GpuApi/Dx12/Dx12Queue.h"
#include "GpuApi/Dx12/Dx12ResourceManager.h"
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

		void CreateCommandObjects();

		void CreateSwapChain();

		void LoadAssets();

		void ResizeViewport();
		void ResizeScissors();

		void OnWindowClose(const WindowCloseCallbackData& callbackData);

		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};

		std::shared_ptr<EventRegistry> eventRegistry;
		std::shared_ptr<WindowWin32> window;

		std::unique_ptr<Dx12Device> device;
		std::unique_ptr<Dx12SwapChain> swapChain;

		std::unique_ptr<Dx12ResourceManager> resourceManager;

		/*
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		Microsoft::WRL::ComPtr<ID3D12Resource> vertexBuffer;
		*/
		std::shared_ptr<Dx12Mesh> mesh;

		std::shared_ptr<Dx12Queue> directQueue;
		Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator;
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList7> graphicsCommandList;

		std::shared_ptr<Dx12Fence> frameCompletedFence;

		std::shared_ptr<Dx12RootSignature> rootSignature;
		std::shared_ptr<Dx12GraphicsPSO> graphicsPSO;

		bool appIsRunning{ false };
	};
}