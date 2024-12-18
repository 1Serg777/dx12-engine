#pragma once

#include "Core/Application.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include <memory>

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

		void OnWindowClose(const WindowCloseCallbackData& callbackData);

		std::shared_ptr<EventRegistry> eventRegistry;
		std::shared_ptr<WindowWin32> window;

		bool appIsRunning{ false };
	};
}