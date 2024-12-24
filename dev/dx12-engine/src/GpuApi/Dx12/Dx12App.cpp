#include "GpuApi/Dx12/Dx12App.h"

#include "Core/Logger.h"
#include "Core/Error.h"

#include <functional>

using namespace Microsoft::WRL;

namespace dxe
{
	// Dx12App

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
		rtvHeap.reset();

		swapChainBuffers.clear();
		swapChain.Reset();

#if defined(DEBUG) || defined(_DEBUG)
		DX12_THROW_IF_NOT_SUCCESS(
			infoQueue->UnregisterMessageCallback(callbackCookie),
			"Failed to unregister the Message Callback Function!");
#endif

		device.Reset();
		adapter.Reset();

		factory.Reset();

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

			Render();
			WaitForFrameToFinish();
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

#if defined(DEBUG) || defined(_DEBUG)
		InitializeDebugCallbackFunction();
#endif

		CreateCommandObjects();

		this->frameBufferCount = 2;
		swapChainBuffers.resize(this->frameBufferCount);
		CreateSwapChain();
		CreateDescriptorHeaps();
		CreateFrameResources();

		CreateSynchronizationObjects();
	}

	void Dx12App::Render()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			commandAllocator->Reset(),
			"Failed to reset the command allocator!");

		DX12_THROW_IF_NOT_SUCCESS(
			graphicsCommandList->Reset(commandAllocator.Get(), nullptr),
			"Failed to reset the graphics command list!");

		graphicsCommandList->RSSetViewports(1, &viewport);
		graphicsCommandList->RSSetScissorRects(1, &scissorRect);

		D3D12_RESOURCE_BARRIER renderTargetBarrier{};
		renderTargetBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		renderTargetBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		renderTargetBarrier.Transition.pResource = swapChainBuffers[frameBufferIndex].Get();
		renderTargetBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		renderTargetBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;	
		renderTargetBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		graphicsCommandList->ResourceBarrier(1, &renderTargetBarrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetDescriptorHandle(frameBufferIndex);
		graphicsCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		graphicsCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

		D3D12_RESOURCE_BARRIER presentBarrier{};
		presentBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		presentBarrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		presentBarrier.Transition.pResource = swapChainBuffers[frameBufferIndex].Get();
		presentBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		presentBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		presentBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		graphicsCommandList->ResourceBarrier(1, &presentBarrier);

		DX12_THROW_IF_NOT_SUCCESS(
			graphicsCommandList->Close(),
			"Failed to close the graphics command list!");

		ID3D12CommandList* ppCommandLists[] = { graphicsCommandList.Get() };
		commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

		DX12_THROW_IF_NOT_SUCCESS(
			swapChain->Present(1, 0),
			"Failed to present a swap chain image!");
	}

	void Dx12App::CreateFactory(UINT factoryFlags)
	{
		// HRESULT factoryCreationResult = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&factory));

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(&factory)),
			"Failed to initialize DXGI Factory!");
		*/

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(factory.ReleaseAndGetAddressOf())),
			"Failed to initialize DXGI Factory!");
		*/

		ComPtr<IDXGIFactory2> tempFactory;
		DX12_THROW_IF_NOT_SUCCESS(
			CreateDXGIFactory2(factoryFlags, IID_PPV_ARGS(tempFactory.ReleaseAndGetAddressOf())),
			"Failed to initialize DXGI Factory!");

		DX12_THROW_IF_NOT_SUCCESS(
			tempFactory.As(&factory),
			"Couldn't acquire an IDXGIFactory7 interface pointer!");
	}

#if defined(DEBUG) || defined(_DEBUG)
	void Dx12App::InitializeDebugLayers()
	{
		// Enable the debug layer (requires the Graphics Tools "optional feature").
		// NOTE: Enabling the debug layer after device creation will invalidate the active device.
		
		// HRESULT hr = D3D12GetDebugInterface(__uuidof(debugController.Get()), (void**)debugController.GetAddressOf());
		// HRESULT hr = D3D12GetDebugInterface(IID_PPV_ARGS(&debugController));

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			D3D12GetDebugInterface(IID_PPV_ARGS(&debugLayer)),
			"Failed to initialize Debug Layers!");
		*/

		/*
		DX12_THROW_IF_NOT_SUCCESS(
			D3D12GetDebugInterface(__uuidof(debugLayer.Get()), (void**)debugLayer.GetAddressOf()),
			"Failed to initialize Debug Layers!");
		*/

		DX12_THROW_IF_NOT_SUCCESS(
			D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.ReleaseAndGetAddressOf())),
			"Failed to initialize Debug Layers!");

		debugLayer->EnableDebugLayer();
		debugLayer->SetEnableGPUBasedValidation(true);
	}
	void Dx12App::InitializeDebugCallbackFunction()
	{
		/*
		DX12_THROW_IF_NOT_SUCCESS(
			device->QueryInterface<ID3D12InfoQueue1>(&infoQueue),
			"Failed to query the ID3D12InfoQueue1 interface!");
		*/

		/*
		auto ptr = reinterpret_cast<ComPtr<ID3D12InfoQueue1>*>(&reinterpret_cast<unsigned char&>(infoQueue));

		DX12_THROW_IF_NOT_SUCCESS(
			device.As(ptr),
			"Failed to query the ID3D12InfoQueue1 interface!");
		*/

		DX12_THROW_IF_NOT_SUCCESS(
			device.As(&infoQueue),
			"Failed to query the ID3D12InfoQueue1 interface!");		

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		DX12_THROW_IF_NOT_SUCCESS(
			infoQueue->RegisterMessageCallback(
				&Dx12App::DebugMessenger, D3D12_MESSAGE_CALLBACK_FLAG_NONE, this, &callbackCookie),
			"Failed to register a Message Callback Function!");
	}
#endif

	void Dx12App::PickAdapter()
	{
		std::vector<ComPtr<IDXGIAdapter4>> adapters;

		uint32_t adapterIdx{ 0 };
		while (true)
		{
			ComPtr<IDXGIAdapter4> adapter;

			HRESULT hr = factory->EnumAdapterByGpuPreference(
				adapterIdx, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));

			if (hr == DXGI_ERROR_NOT_FOUND)
				break;

			DX12_THROW_IF_NOT_SUCCESS(
				hr,
				"Unable to retrieve an adapter!");

			adapters.push_back(std::move(adapter));

			adapterIdx++;
		}

		adapterIdx = 0;
		for (ComPtr<IDXGIAdapter4>& adapter : adapters)
		{
			DXGI_ADAPTER_DESC3 adapterDesc{};
			adapter->GetDesc3(&adapterDesc);

			std::wstringstream logStream{};
			LogAdapter(logStream, adapterIdx, adapterDesc);
			// LogAdapterOutputs(logStream, adapter);
			Logger::Info(logStream.str());

			if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
			{
				continue;
			}

			// Check DX 12 support without creating a device for the adapter!
			// Essentially, we pick the first non-software adapter that supports DX12.
			// Since we enumerate adapters giving preference to high performance ones,
			// the system's discrete (or external) GPU adapter will precede the integrated one.
			// As such we'll end up picking it while also making sure that all of the adapters are properly logged.
			if (!this->adapter &&
				SUCCEEDED(D3D12CreateDevice(
					adapter.Get(), D3D_FEATURE_LEVEL_12_2, _uuidof(ID3D12Device), nullptr)))
			{
				this->adapter = adapter;
			}
			
			adapterIdx++;
		}
	}
	void Dx12App::LogAdapter(std::wstringstream& logStream, UINT adapterIdx, DXGI_ADAPTER_DESC3& adapterDesc)
	{
		logStream << "\n";
		logStream << L"Adapter [" << adapterIdx << L"]\n";
		logStream << L"Adapter: " << adapterDesc.Description << L"\n";
		logStream << L"Private System memory (bytes): " << adapterDesc.DedicatedSystemMemory << L"\n";
		logStream << L"Shared System memory (bytes): " << adapterDesc.SharedSystemMemory << L"\n";
		logStream << L"Video memory (bytes): " << adapterDesc.DedicatedVideoMemory;
	}
	void Dx12App::LogAdapterOutputs(std::wstringstream& logStream, ComPtr<IDXGIAdapter4> adapter)
	{
		for (UINT i = 0; true; i++)
		{
			ComPtr<IDXGIOutput> output = nullptr;
			ComPtr<IDXGIOutput6> outputVer6 = nullptr;
			HRESULT hr = adapter->EnumOutputs(i, output.GetAddressOf());

			if (hr == DXGI_ERROR_NOT_FOUND)
				break;

			DX12_THROW_IF_NOT_SUCCESS(
				output.As<IDXGIOutput6>(&outputVer6),
				"Couldn't cast IDXGIOutput to IDXGIOutput6!");

			DXGI_OUTPUT_DESC1 outputDesc{};
			outputVer6->GetDesc1(&outputDesc);

			logStream << "\n";
			logStream << L"Output [" << i << L"]\n";
			logStream << L"Device name: " << outputDesc.DeviceName;

			// LogOutputDisplayModes(logStream, outputVer6);
		}
	}
	void Dx12App::LogOutputDisplayModes(std::wstringstream& logStream, ComPtr<IDXGIOutput6> output)
	{
		UINT displayModeCount{ 0 };
		UINT displayModeFlags{ 0 };

		output->GetDisplayModeList1(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			displayModeFlags,
			&displayModeCount,
			nullptr);

		std::vector<DXGI_MODE_DESC1> displayModes(displayModeCount);

		output->GetDisplayModeList1(
			DXGI_FORMAT_R8G8B8A8_UNORM,
			displayModeFlags,
			&displayModeCount,
			displayModes.data());

		for (UINT i = 0; i < displayModeCount; i++)
		{
			DXGI_MODE_DESC1 mode = displayModes[i];

			logStream << "\n";
			logStream << L"Display mode [" << i << L"]\n";
			logStream << L"Width = " << mode.Width << L"\n";
			logStream << L"Height = " << mode.Height << L"\n";
			logStream << L"Refresh rate = " <<
				mode.RefreshRate.Numerator << " / " <<
				mode.RefreshRate.Denominator << L"\n";
		}
	}

	void Dx12App::CreateDevice()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(device.ReleaseAndGetAddressOf())),
			"Unable to create a D3D12 device that supports the 12.2 feature level!");
	}

	void Dx12App::CreateCommandObjects()
	{
		CreateCommandQueue();
		CreateCommandAllocators();
		CreateCommandLists();
	}
	void Dx12App::CreateCommandQueue()
	{
		D3D12_COMMAND_QUEUE_DESC queueDesc = {};
		queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
		queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandQueue(
				&queueDesc,
				IID_PPV_ARGS(commandQueue.ReleaseAndGetAddressOf())),
			"Failed to create a Command Queue!");
	}
	void Dx12App::CreateCommandAllocators()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandAllocator(
				D3D12_COMMAND_LIST_TYPE_DIRECT,
				IID_PPV_ARGS(commandAllocator.ReleaseAndGetAddressOf())),
			"Failed to create a Command Allocator!");
	}
	void Dx12App::CreateCommandLists()
	{
		/*
		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandList1(0, D3D12_COMMAND_LIST_TYPE_DIRECT, ,),
			"Failed to create a Command Allocator!");

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, , ),
			"Failed to create a Command Allocator!");
		*/

		graphicsCommandList = CreateGraphicsCommandList(commandAllocator.Get());
	}
	ComPtr<ID3D12GraphicsCommandList7> Dx12App::CreateGraphicsCommandList(ID3D12CommandAllocator* commandAllocator)
	{
		ComPtr<ID3D12GraphicsCommandList7> commandList;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandList(
				0, D3D12_COMMAND_LIST_TYPE_DIRECT,
				commandAllocator, nullptr,
				IID_PPV_ARGS(commandList.GetAddressOf())),
			"Failed to create a Graphics Command List!");

		commandList->Close();

		return commandList;
	}

	void Dx12App::CreateSwapChain()
	{
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
		swapChainDesc.BufferCount = frameBufferCount;
		swapChainDesc.Width = window->GetWindowWidth();
		swapChainDesc.Height = window->GetWindowHeight();
		swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapChainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapChain;
		HRESULT hr = factory->CreateSwapChainForHwnd(
			commandQueue.Get(),
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

		frameBufferIndex = this->swapChain->GetCurrentBackBufferIndex();
	}

	void Dx12App::CreateDescriptorHeaps()
	{
		CreateRTVDescriptorHeap();
	}
	void Dx12App::CreateRTVDescriptorHeap()
	{
		rtvHeap = std::make_shared<Dx12DescriptorHeap>(frameBufferCount, D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		rtvHeap->Initialize(device.Get());
	}

	void Dx12App::CreateFrameResources()
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

	void Dx12App::CreateSynchronizationObjects()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateFence(
				0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(fence.ReleaseAndGetAddressOf())),
			"Failed to create a Fence object!");

		fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
		WINAPI_THROW_IF_NULL(fenceEvent, "Failed to create a Fence Event!");

		// fenceValue++;
		fenceValue = 1;
	}

	void Dx12App::WaitForFrameToFinish()
	{
		const UINT64 fv = fenceValue;

		DX12_THROW_IF_NOT_SUCCESS(
			commandQueue->Signal(fence.Get(), fv),
			"Failed to signal a fence value!");

		fenceValue++;

		if (fence->GetCompletedValue() < fv)
		{
			DX12_THROW_IF_NOT_SUCCESS(
				fence->SetEventOnCompletion(fv, fenceEvent),
				"Couldn't set the fence event!");

			WaitForSingleObject(fenceEvent, INFINITE);
		}

		frameBufferIndex = swapChain->GetCurrentBackBufferIndex();
	}

	void Dx12App::CreateViewport()
	{
		viewport.TopLeftX = 0.0f;
		viewport.TopLeftY = 0.0f;

		viewport.Width = static_cast<float>(window->GetWindowWidth());
		viewport.Height = static_cast<float>(window->GetWindowHeight());

		viewport.MinDepth = D3D12_MIN_DEPTH;
		viewport.MaxDepth = D3D12_MAX_DEPTH;
	}
	void Dx12App::CreateScissors()
	{
		scissorRect.left = 0;
		scissorRect.right = static_cast<LONG>(window->GetWindowWidth());
		scissorRect.top = 0;
		scissorRect.bottom = static_cast<LONG>(window->GetWindowHeight());
	}

	void Dx12App::OnWindowClose(const WindowCloseCallbackData& callbackData)
	{
		Logger::Info("Closing the application...");
		appIsRunning = false;
	}

	void Dx12App::DebugMessenger(
		D3D12_MESSAGE_CATEGORY Category,
		D3D12_MESSAGE_SEVERITY Severity,
		D3D12_MESSAGE_ID ID,
		LPCSTR pDescription,
		void* pContext)
	{
		Dx12App* app = static_cast<Dx12App*>(pContext);
		Logger::Error(pDescription);
	}
}