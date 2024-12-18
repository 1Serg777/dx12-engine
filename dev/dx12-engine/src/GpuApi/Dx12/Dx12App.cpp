#include "GpuApi/Dx12/Dx12App.h"

#include "Core/Logger.h"
#include "Core/Error.h"

#include <functional>
#include <sstream>
#include <iostream>

namespace dxe
{
	void Dx12App::Initialize()
	{
		Logger::Initialize();
		Logger::Info("Initializing DX12 Engine...");

		using namespace std::placeholders;

		eventRegistry = std::make_shared<EventRegistry>();
		eventRegistry->RegisterCallback<dxe::WindowCloseCallbackData>(
			std::bind(&Dx12App::OnWindowClose, this, _1));

		InitializeWindow();
		InitializeDx12();

		Logger::Info("DX12 Engine initialization complete!");
	}
	void Dx12App::Terminate()
	{
		window.reset();
		eventRegistry.reset();
		Logger::Terminate();
	}

	void Dx12App::Run()
	{
		appIsRunning = true;
		while (appIsRunning)
		{
			window->Tick();
			eventRegistry->Tick();
		}

		// Logger::Info("Done!");
	}

	void Dx12App::InitializeWindow()
	{
		Logger::Info("Initializing Win32 window...");

		dxe::WindowSettings defWindowSettings{};
		window = std::make_shared<WindowWin32>(defWindowSettings);

		window->SetEventRegistry(eventRegistry.get());
		window->InitializeWin32WindowParams();
		window->CreateWin32Window();
		window->SetWindowVisibility(true);

		Logger::Info("Win32 window initialization complete!");
	}

	void Dx12App::InitializeDx12()
	{
		UINT dxgiFactoryFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
		InitializeDebugLayers();
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif

		CreateFactory(dxgiFactoryFlags);

		PickAdapter();
		CreateDevice();
	}

	void Dx12App::CreateFactory(UINT factoryFlags)
	{
		// HRESULT factoryCreationResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

		DX12_THROW_IF_NOT_SUCCESS(
			CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)),
			"Failed to initialize DXGI Factory!");
	}

#if defined(DEBUG) || defined(_DEBUG)
	void Dx12App::InitializeDebugLayers()
	{
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		
		// HRESULT hr = D3D12GetDebugInterface(__uuidof(debugController.Get()), (void**)debugController.GetAddressOf());
		// HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));

		DX12_THROW_IF_NOT_SUCCESS(
			D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)),
			"Failed to initialize Debug Layers!");

		debugLayer->EnableDebugLayer();
		debugLayer->SetEnableGPUBasedValidation(true);
	}
#endif

	void Dx12App::PickAdapter()
	{
		std::vector<ComPtr<IDXGIAdapter4>> adapters;

		for (UINT i = 0; true; i++)
		{
			ComPtr<IDXGIAdapter1> adapter = nullptr;
			HRESULT hr = factory->EnumAdapters1(i, adapter.GetAddressOf());

			if (hr == DXGI_ERROR_NOT_FOUND)
				break;

			DX12_THROW_IF_NOT_SUCCESS(
				hr,
				"Was unable to retrieve an adapter!");

			ComPtr<IDXGIAdapter4> adapterVer4 = nullptr;
			DX12_THROW_IF_NOT_SUCCESS(
				adapter.As<IDXGIAdapter4>(&adapterVer4),
				"Couldn't cast IDXGIAdapter1 to IDXGIAdapter4 while retrieving a list of available adapters in the system!");

			adapters.push_back(std::move(adapterVer4));
		}

		// Print adapter info for each available adapter

		uint32_t adapterIdx{ 0 };
		for (ComPtr<IDXGIAdapter4>& adapter : adapters)
		{
			DXGI_ADAPTER_DESC3 adapterDesc{};
			adapter->GetDesc3(&adapterDesc);

			/*
			std::stringstream adapterLogStream;

			adapterLogStream << "Adapter [" << adapterIdx << "]\n";
			adapterLogStream << "Adapter: " << adapterDesc.Description << "\n";
			adapterLogStream << "System memory (bytes): " << adapterDesc.DedicatedSystemMemory << "\n";
			adapterLogStream << "Video memory (bytes): " << adapterDesc.DedicatedVideoMemory << "\n";

			Logger::Info(adapterLogStream.str());
			*/
			
			std::wstringstream adapterLogStream;

			adapterLogStream << "\n";
			adapterLogStream << L"Adapter [" << adapterIdx << L"]\n";
			adapterLogStream << L"Adapter: " << adapterDesc.Description << L"\n";
			adapterLogStream << L"System memory (bytes): " << adapterDesc.DedicatedSystemMemory << L"\n";
			adapterLogStream << L"Video memory (bytes): " << adapterDesc.DedicatedVideoMemory;

			// OutputDebugString(adapterLogStream.str().c_str());
			// std::wcerr << adapterLogStream.str();
			Logger::Info(adapterLogStream.str());

			adapterIdx++;
		}
	}
	void Dx12App::CreateDevice()
	{
		// TODO
	}

	void Dx12App::OnWindowClose(const WindowCloseCallbackData& callbackData)
	{
		Logger::Info("Closing the application...");
		appIsRunning = false;
	}
}