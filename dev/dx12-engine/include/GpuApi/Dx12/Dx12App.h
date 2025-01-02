#pragma once

#include "Core/Application.h"
#include "Events/EventRegistry.h"
#include "Window/WindowWin32.h"

#include <cstdint>
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

		void LoadAssets();

		void Render();

		void OnWindowClose(const WindowCloseCallbackData& callbackData);

		std::shared_ptr<EventRegistry> eventRegistry;
		std::shared_ptr<WindowWin32> window;

		uint32_t meshId{ 0 };
		uint32_t rootSignatureId{ 0 };
		uint32_t graphicsPSOId{ 0 };

		bool appIsRunning{ false };
	};
}