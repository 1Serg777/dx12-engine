#include "GpuApi/Dx12/Dx12App.h"

#include "Core/Logger.h"

#include <functional>

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

	void Dx12App::OnWindowClose(const WindowCloseCallbackData& callbackData)
	{
		Logger::Info("Closing the application...");
		appIsRunning = false;
	}
}