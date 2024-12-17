#include "Window/Window.h"

namespace dxe
{
	Window::Window(const WindowSettings& windowSettings)
		: windowSettings(windowSettings)
	{
	}
	Window::~Window()
	{
		RemoveEventRegistry();
	}

	void Window::SetEventRegistry(EventRegistry* eventRegistry)
	{
		this->eventRegistry = eventRegistry;
	}
	void Window::RemoveEventRegistry()
	{
		this->eventRegistry = nullptr;
	}

	const std::string& Window::GetWindowTitle() const
	{
		return windowSettings.windowTitle;
	}

	const Dimensions2D& Window::GetWindowDimensions() const
	{
		return windowSettings.windowDimensions;
	}

	uint32_t Window::GetWindowWidth() const
	{
		return windowSettings.windowDimensions.width;
	}
	uint32_t Window::GetWindowHeight() const
	{
		return windowSettings.windowDimensions.height;
	}

	bool Window::IsFullScreen() const
	{
		return windowSettings.isFullscreen;
	}

	bool Window::IsVisible() const
	{
		return windowSettings.isVisible;
	}
}