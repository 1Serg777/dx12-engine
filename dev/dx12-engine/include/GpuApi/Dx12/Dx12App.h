#pragma once

#include "Core/Application.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include <d3d12.h>
#include <dxgi1_6.h>

#include <memory>

#include <wrl.h>
using Microsoft::WRL::ComPtr;

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

		void CreateFactory(UINT factoryFlags);
#if defined(DEBUG) || defined(_DEBUG)
		void InitializeDebugLayers();
		ComPtr<ID3D12Debug6> debugLayer;
#endif

		void PickAdapter();
		void CreateDevice();

		void OnWindowClose(const WindowCloseCallbackData& callbackData);

		std::shared_ptr<EventRegistry> eventRegistry;
		std::shared_ptr<WindowWin32> window;

		ComPtr<IDXGIAdapter4> adapter;
		ComPtr<IDXGIFactory7> factory;

		bool appIsRunning{ false };
	};
}