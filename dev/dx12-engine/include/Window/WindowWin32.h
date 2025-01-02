#pragma once

#include "Core/Utility.h"
#include "Window/Window.h"

#include <Windows.h>
#undef CreateWindow
#undef min
#undef max

#include <cstdint>
#include <string>

namespace dxe
{
	class WindowWin32 : public Window
	{
	public:

		WindowWin32(const WindowSettings& windowSettings);
		virtual ~WindowWin32() = default;

		CLASS_NO_COPY(WindowWin32);
		CLASS_DEFAULT_MOVE(WindowWin32);

		void Tick() override;

		void InitializeWin32WindowParams();
		void CreateWin32Window();

		void SetWindowVisibility(bool showWindow);

		void DestroyWin32Window();
		void TerminateWin32WindowParams();

		const HWND& GetWindowHandle() const;

	protected:

		void CreateWindowClass();
		void CreateWindowRect(RECT& windowRect);

		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		LRESULT WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:

		HWND hWnd{};

		bool inFullscreenMode{ false };
		bool isResizing{ false };
	};

	WindowWin32* GetWindowWin32();
}