#include "Core/Logger.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include <cstdlib>
#include <functional>

int main()
{
	dxe::Logger::Initialize();
	dxe::Logger::Info("Hello DX12 Engine!");

	dxe::EventRegistry eventRegistry{};

	dxe::WindowSettings defWindowSettings{};
	dxe::WindowWin32 window{ defWindowSettings };
	window.SetEventRegistry(&eventRegistry);
	window.InitializeWin32WindowParams();
	window.CreateWin32Window();
	window.SetWindowVisibility(true);

	bool appIsRunning{ false };
	auto closeEventCallback = [&appIsRunning](const dxe::WindowCloseCallbackData& data) {
		appIsRunning = false;
	};

	eventRegistry.RegisterCallback<dxe::WindowCloseCallbackData>(closeEventCallback);

	appIsRunning = true;
	while (appIsRunning)
	{
		window.Tick();
		eventRegistry.Tick();
	}

	dxe::Logger::Terminate();
	return EXIT_SUCCESS;
}