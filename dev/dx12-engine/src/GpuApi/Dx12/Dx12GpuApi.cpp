#include "GpuApi/Dx12/Dx12GpuApi.h"

#include "GpuApi/Dx12/Dx12CommandManager.h"
#include "GpuApi/Dx12/Dx12Device.h"
#include "GpuApi/Dx12/Dx12ResourceManager.h"
#include "GpuApi/Dx12/Dx12SwapChain.h"

#include "Renderer/Dx12/Dx12Renderer.h"

#include "Window/WindowWin32.h"

#include <cassert>

namespace dxe
{
	// [TODO]
	// Would be a nice idea to allocate one chunk of memory in the heap
	// that would be capable of containing both the 'Dx12GpuData' pointers
	// and the data of the corresponding objects.
	// [TODO]

	static Dx12GpuData* gpuData{ nullptr };

	static uint32_t frameBufferCount{ 2 };

	static void CreateDevice();
	static void DestroyDevice();

	static void CreateCommandManager();
	static void DestroyCommandManager();

	static void CreateSwapChain();
	static void DestroySwapChain();

	static void CreateResourceManager();
	static void DestroyResourceManager();

	static void CreateRenderer();
	static void DestroyRenderer();

	Dx12GpuData* GetDx12GpuData()
	{
		return gpuData;
	}

	void InitializeDx12()
	{
		assert(!gpuData && "GPU API can only be initialized once!");
		gpuData = new Dx12GpuData();

		CreateDevice();
		CreateCommandManager();
		CreateSwapChain();
		CreateResourceManager();
		CreateRenderer();
	}
	void TerminateDx12()
	{
		DestroyRenderer();
		DestroyResourceManager();
		DestroySwapChain();
		DestroyCommandManager();
		DestroyDevice();

		delete gpuData;
	}

	static void CreateDevice()
	{
		gpuData->device = new Dx12Device();
		gpuData->device->Initialize();
	}
	void DestroyDevice()
	{
		gpuData->device->Terminate();

		delete gpuData->device;
		gpuData->device = nullptr;
	}

	void CreateCommandManager()
	{
		gpuData->commandManager = new Dx12CommandManager();
		gpuData->commandManager->CreateCommandQueues(gpuData->device->GetDevice());
		gpuData->commandManager->CreateCommandContexts(gpuData->device->GetDevice(), frameBufferCount);
	}
	void DestroyCommandManager()
	{
		gpuData->commandManager->DestroyCommandContexts();
		gpuData->commandManager->DestroyCommandQueues();

		delete gpuData->commandManager;
		gpuData->commandManager = nullptr;
	}

	static void CreateSwapChain()
	{
		WindowWin32* window = GetWindowWin32();
		assert(window && "Win32 window pointer was 'nullptr'! "
			"Only initialize DX12 after you've successfully initialized Win32 API!");

		gpuData->swapChain = new Dx12SwapChain(frameBufferCount);
		gpuData->swapChain->CreateSwapChain(
			gpuData->device->GetDevice(),
			gpuData->device->GetFactory(),
			gpuData->commandManager->GetDirectQueue()->GetCommandQueue(),
			window);
	}
	void DestroySwapChain()
	{
		delete gpuData->swapChain;
		gpuData->swapChain = nullptr;
	}

	static void CreateResourceManager()
	{
		gpuData->resourceManager = new Dx12ResourceManager();
		gpuData->resourceManager->Initialize();
	}
	static void DestroyResourceManager()
	{
		gpuData->resourceManager->Terminate();

		delete gpuData->resourceManager;
		gpuData->resourceManager = nullptr;
	}

	static void CreateRenderer()
	{
		gpuData->renderer = new Dx12Renderer();
		gpuData->renderer->Initialize();

		WindowWin32* window = GetWindowWin32();
		assert(window && "Win32 window pointer was 'nullptr'! "
			"Only initialize DX12 after you've successfully initialized Win32 API!");

		gpuData->renderer->SetViewport(window->GetWindowWidth(), window->GetWindowHeight());
		gpuData->renderer->SetScissors(window->GetWindowWidth(), window->GetWindowHeight());
	}
	static void DestroyRenderer()
	{
		gpuData->renderer->Terminate();

		delete gpuData->renderer;
		gpuData->renderer = nullptr;
	}
}