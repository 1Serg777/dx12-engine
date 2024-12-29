#include "GpuApi/Dx12/Dx12Device.h"

#include "Core/Error.h"
#include "Core/Logger.h"

#include <sstream>

using namespace Microsoft::WRL;

namespace dxe
{
	void Dx12Device::Initialize()
	{
#if defined(DEBUG) || defined(_DEBUG)
		InitializeDebugLayers();
#endif

		CreateFactory();

		PickAdapter();
		CreateDevice();

#if defined(DEBUG) || defined(_DEBUG)
		InitializeDebugCallbackFunction();
#endif
	}

	void Dx12Device::Terminate()
	{
#if defined(DEBUG) || defined(_DEBUG)
		DX12_THROW_IF_NOT_SUCCESS(
			infoQueue->UnregisterMessageCallback(debugMsgCallbackCookie),
			"Failed to unregister the Message Callback Function!");
#endif

		device.Reset();
		adapter.Reset();
		factory.Reset();
	}

	std::shared_ptr<Dx12DirectQueue> Dx12Device::CreateDirectQueue()
	{
		std::shared_ptr<Dx12DirectQueue> directQueue = std::make_shared<Dx12DirectQueue>();
		directQueue->InitializeCommandQueue(device.Get());
		return directQueue;
	}
	std::shared_ptr<Dx12ComputeQueue> Dx12Device::CreateComputeQueue()
	{
		std::shared_ptr<Dx12ComputeQueue> computeQueue = std::make_shared<Dx12ComputeQueue>();
		computeQueue->InitializeCommandQueue(device.Get());
		return computeQueue;
	}

	ComPtr<ID3D12CommandAllocator> Dx12Device::CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE allocatorType) const
	{
		ComPtr<ID3D12CommandAllocator> commandAllocator;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandAllocator(
				allocatorType,
				IID_PPV_ARGS(commandAllocator.GetAddressOf())),
			"Failed to create a Command Allocator!");

		return commandAllocator;
	}

	ComPtr<ID3D12GraphicsCommandList7> Dx12Device::CreateGraphicsCommandList(
		D3D12_COMMAND_LIST_TYPE cmdListType, ID3D12CommandAllocator* commandAllocator) const
	{
		ComPtr<ID3D12GraphicsCommandList7> commandList;

		DX12_THROW_IF_NOT_SUCCESS(
			device->CreateCommandList(
				0, cmdListType,
				commandAllocator, nullptr,
				IID_PPV_ARGS(commandList.GetAddressOf())),
			"Failed to create a Graphics Command List!");

		commandList->Close();

		return commandList;
	}

	IDXGIFactory7* Dx12Device::GetFactory() const
	{
		return factory.Get();
	}

	IDXGIAdapter4* Dx12Device::GetAdapter() const
	{
		return adapter.Get();
	}
	ID3D12Device10* Dx12Device::GetDevice() const
	{
		return device.Get();
	}

	void Dx12Device::CreateFactory()
	{
		UINT dxgiFactoryFlags{ 0 };

#if defined(DEBUG) || defined(_DEBUG)
		dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
#endif	

		ComPtr<IDXGIFactory2> tempFactory;
		DX12_THROW_IF_NOT_SUCCESS(
			CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(tempFactory.GetAddressOf())),
			"Failed to initialize the DXGI Factory!");

		DX12_THROW_IF_NOT_SUCCESS(
			tempFactory.As(&factory),
			"Failed to acquire an IDXGIFactory7 interface pointer!");
	}

#if defined(DEBUG) || defined(_DEBUG)
	void Dx12Device::InitializeDebugLayers()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			D3D12GetDebugInterface(IID_PPV_ARGS(debugLayer.ReleaseAndGetAddressOf())),
			"Failed to initialize the Debug Layer!");

		debugLayer->EnableDebugLayer();
		debugLayer->SetEnableGPUBasedValidation(true);
	}
	void Dx12Device::InitializeDebugCallbackFunction()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			device.As(&infoQueue),
			"Failed to query the ID3D12InfoQueue1 interface!");

		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);

		DX12_THROW_IF_NOT_SUCCESS(
			infoQueue->RegisterMessageCallback(
				&Dx12Device::DebugMessenger, D3D12_MESSAGE_CALLBACK_FLAG_NONE, this, &debugMsgCallbackCookie),
			"Failed to register a Message Callback Function!");
	}
#endif

	void Dx12Device::PickAdapter()
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

	void Dx12Device::LogAdapter(std::wstringstream& logStream, UINT adapterIdx, DXGI_ADAPTER_DESC3& adapterDesc)
	{
		logStream << "\n";
		logStream << L"Adapter [" << adapterIdx << L"]\n";
		logStream << L"Adapter: " << adapterDesc.Description << L"\n";
		logStream << L"Private System memory (bytes): " << adapterDesc.DedicatedSystemMemory << L"\n";
		logStream << L"Shared System memory (bytes): " << adapterDesc.SharedSystemMemory << L"\n";
		logStream << L"Video memory (bytes): " << adapterDesc.DedicatedVideoMemory;
	}
	void Dx12Device::LogAdapterOutputs(std::wstringstream& logStream, ComPtr<IDXGIAdapter4> adapter)
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
	void Dx12Device::LogOutputDisplayModes(std::wstringstream& logStream, ComPtr<IDXGIOutput6> output)
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

	void Dx12Device::CreateDevice()
	{
		DX12_THROW_IF_NOT_SUCCESS(
			D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(device.ReleaseAndGetAddressOf())),
			"Unable to create a D3D12 device that supports the 12.2 feature level!");
	}

	void Dx12Device::DebugMessenger(
		D3D12_MESSAGE_CATEGORY Category,
		D3D12_MESSAGE_SEVERITY Severity,
		D3D12_MESSAGE_ID ID,
		LPCSTR pDescription,
		void* pContext)
	{
		Dx12Device* device = static_cast<Dx12Device*>(pContext);
		Logger::Error(pDescription);
	}
}