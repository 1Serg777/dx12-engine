#pragma once

#include <cstdint>

namespace dxe
{
	class Dx12CommandManager;
	class Dx12Device;
	class Dx12Renderer;
	class Dx12ResourceManager;
	class Dx12SwapChain;

	struct Dx12GpuData
	{
		Dx12CommandManager*  commandManager  { nullptr };
		Dx12Device*          device          { nullptr };
		Dx12Renderer*        renderer        { nullptr };
		Dx12ResourceManager* resourceManager { nullptr };
		Dx12SwapChain*       swapChain       { nullptr };
	};

	Dx12GpuData* GetDx12GpuData();

	void InitializeDx12();
	void TerminateDx12();

	// [TODO]
	// 
	// void ResizeSwapChain(uint32_t width, uint32_t height);
	// void ChangeBackBufferCount(uint32_t backBufferCount);
	// 
	// [TODO]
}