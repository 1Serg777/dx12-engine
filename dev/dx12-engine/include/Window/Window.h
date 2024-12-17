#pragma once

#include "Core/Utility.h"
#include "Core/MathUtility.h"

#include "Events/EventRegistry.h"

#include <cstdint>
#include <string>

namespace dxe
{
	struct WindowSettings
	{
		std::string windowTitle{ "DX12 Engine" };

		Dimensions2D windowDimensions{ 1920, 1080 };
		Dimensions2D framebufferDimensions{ 1920, 1080 };

		bool isFullscreen{ false };
		bool isVisible{ true };
	};

	class Window
	{
	public:

		Window(const WindowSettings& windowSettings);
		virtual ~Window();

		virtual void Tick() = 0;

		virtual void SetEventRegistry(EventRegistry* eventRegistry);
		virtual void RemoveEventRegistry();

		const std::string& GetWindowTitle() const;

		const Dimensions2D& GetWindowDimensions() const;

		uint32_t GetWindowWidth() const;
		uint32_t GetWindowHeight() const;

		bool IsFullScreen() const;
		bool IsVisible() const;

	protected:

		WindowSettings windowSettings{};

		EventRegistry* eventRegistry{ nullptr };
	};
}