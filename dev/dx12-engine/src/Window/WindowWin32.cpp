#include "Window/WindowWin32.h"

#include "Core/Error.h"

#include <cassert>

namespace dxe
{
	static WindowWin32* windowPtr = nullptr;

	WindowWin32::WindowWin32(const WindowSettings& windowSettings)
		: Window(windowSettings)
	{
	}

	void WindowWin32::Tick()
	{
		MSG msg = { 0 };

		if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
	}

	void WindowWin32::InitializeWin32WindowParams()
	{
		CreateWindowClass();
	}
	void WindowWin32::CreateWin32Window()
	{
		RECT windowRect{};
		CreateWindowRect(windowRect);

		this->hWnd = CreateWindowExA(
			WS_EX_OVERLAPPEDWINDOW,
			windowSettings.windowTitle.c_str(), windowSettings.windowTitle.c_str(),
			WS_OVERLAPPEDWINDOW,
			windowRect.left, windowRect.top, windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
			NULL, NULL, GetModuleHandle(nullptr), this);

		WINAPI_NULL_VALUE_ERROR(this->hWnd, "HWND handle was NULL!");
	}

	void WindowWin32::SetWindowVisibility(bool showWindow)
	{
		windowSettings.isVisible = showWindow;

		if (windowSettings.isVisible)
		{
			ShowWindow(this->hWnd, SW_SHOW);
		}
		else
		{
			ShowWindow(this->hWnd, SW_HIDE);
		}
	}

	void WindowWin32::DestroyWin32Window()
	{
		DestroyWindow(hWnd);
	}
	void WindowWin32::TerminateWin32WindowParams()
	{
		UnregisterClassA(windowSettings.windowTitle.c_str(), GetModuleHandleA(nullptr));
	}

	const HWND& WindowWin32::GetWindowHandle() const
	{
		return this->hWnd;
	}

	void WindowWin32::CreateWindowClass()
	{
		WNDCLASSEXA wc{};
		wc.cbSize = sizeof(WNDCLASSEXA);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		wc.lpfnWndProc = &WindowWin32::WindowProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandleA(nullptr);
		wc.hIcon = NULL; // Use default for now
		wc.hCursor = NULL; // Use default for now
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = windowSettings.windowTitle.c_str();
		wc.hIconSm = NULL; // The same as the hIcon member

		WINAPI_ZERO_VALUE_ERROR(RegisterClassExA(&wc), "WNDCLASSEXA couldn't be created!");

		// The name of the window class is the same as the window title.
	}
	void WindowWin32::CreateWindowRect(RECT& windowRect)
	{
		int screenWidth = GetSystemMetrics(SM_CXSCREEN);
		int screenHeight = GetSystemMetrics(SM_CYSCREEN);

		// Window in center of the screen
		windowRect.left = (screenWidth - GetWindowWidth()) / 2;
		windowRect.top = (screenHeight - GetWindowHeight()) / 2;
		windowRect.right = windowRect.left + GetWindowWidth();
		windowRect.bottom = windowRect.top + GetWindowHeight();

		AdjustWindowRectEx(
			&windowRect,
			WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
			FALSE,
			WS_EX_OVERLAPPEDWINDOW);
	}

	LRESULT CALLBACK WindowWin32::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CREATE:
		{
			LPCREATESTRUCTA lpCreateStructA = (LPCREATESTRUCTA)lParam;
			windowPtr = (WindowWin32*)lpCreateStructA->lpCreateParams;
		}
		return 0;
		default:
		{
			if (windowPtr == nullptr)
				return DefWindowProcA(hWnd, uMsg, wParam, lParam);
			else
				return windowPtr->WindowProcedure(hWnd, uMsg, wParam, lParam);
		}
		}
		return 0;
	}

	LRESULT WindowWin32::WindowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_LBUTTONDOWN:
		{
			/*
			ShowCursor(false);
			SetCapture(this->hWnd);

			POINT currentCursorPos;
			GetCursorPos(&currentCursorPos);

			mouseLockHackPos.x = currentCursorPos.x;
			mouseLockHackPos.y = currentCursorPos.y;

			InputSystem::OnMouseButtonClicked(VK_LBUTTON, currentCursorPos.x, currentCursorPos.y);
			*/
		}
		break;

		case WM_LBUTTONUP:
		{
			/*
			ShowCursor(true);
			ReleaseCapture();

			int xCoord = mouseLockHackPos.x;
			int yCoord = mouseLockHackPos.y;

			SetCursorPos(xCoord, yCoord);

			InputSystem::OnMouseButtonReleased(VK_LBUTTON, xCoord, yCoord);
			*/
		}
		break;

		case WM_MBUTTONDOWN:
		{
			/*
			int xCoord = LOWORD(lParam);
			int yCoord = HIWORD(lParam);
			InputSystem::OnMouseButtonClicked(VK_MBUTTON, xCoord, yCoord);
			break;
			*/
		}
		case WM_MBUTTONUP:
		{
			/*

			int xCoord = LOWORD(lParam);
			int yCoord = HIWORD(lParam);
			InputSystem::OnMouseButtonReleased(VK_MBUTTON, xCoord, yCoord);
			break;

			*/
		}

		case WM_RBUTTONDOWN:
		{
			/*

			ShowCursor(false);
			SetCapture(this->hWnd);

			//int xCoord = LOWORD(lParam);
			//int yCoord = HIWORD(lParam);

			POINT currentCursorPos;
			GetCursorPos(&currentCursorPos);

			mouseLockHackPos.x = currentCursorPos.x;
			mouseLockHackPos.y = currentCursorPos.y;

			InputSystem::OnMouseButtonClicked(VK_RBUTTON, currentCursorPos.x, currentCursorPos.y);
			break;

			*/
		}
		case WM_RBUTTONUP:
		{
			/*

			ShowCursor(true);
			ReleaseCapture();

			// int xCoord = LOWORD(lParam);
			// int yCoord = HIWORD(lParam);

			int xCoord = mouseLockHackPos.x;
			int yCoord = mouseLockHackPos.y;

			SetCursorPos(xCoord, yCoord);

			InputSystem::OnMouseButtonReleased(VK_RBUTTON, xCoord, yCoord);
			break;

			*/
		}

		case WM_KEYUP:
		{
			/*

			InputSystem::OnKeyReleased(wParam);

			*/
		}
		break;
		case WM_KEYDOWN:
		{
			/*

			// Handle Alt-Enter in the unique way
			if (wParam == VK_RETURN && InputManager::IsKeyPressed(VirtualKey::GP_ALT))
			{
				windowPtr->SetResizingState(false);
			}
			InputSystem::OnKeyPressed(wParam);

			*/
		}
		break;

		case WM_MOUSEWHEEL:
		{
			/*

			int wheelDelta = GET_WHEEL_DELTA_WPARAM(wParam);

			int xMouse = GET_X_LPARAM(lParam);
			int yMouse = GET_Y_LPARAM(lParam);

			InputSystem::OnMouseWheel(xMouse, yMouse, wheelDelta);

			*/
		}
		break;

		case WM_MOUSEMOVE:
		{
			// set some condition to mouse movement tracking
			// otherwise it'll slow down app (or may be not)

			/*

			if (true)
			{
				int xMouse = GET_X_LPARAM(lParam);
				int yMouse = GET_Y_LPARAM(lParam);

				InputSystem::OnMouseMove(xMouse, yMouse);
			}
			else
				DefWindowProcW(hWnd, uMsg, wParam, lParam);

			*/
		}
		break;
		case WM_INPUT:
		{
			/*

			HRAWINPUT hRawInput = reinterpret_cast<HRAWINPUT>(lParam);
			UINT sizeInBytes;

			UINT ret = GetRawInputData(hRawInput, RID_INPUT, NULL, &sizeInBytes, sizeof(RAWINPUTHEADER));
			// if (ret == (UINT)-1)
			if (ret != 0)
			{
				Log::LogMessage(L"Cannot get Raw Input Data!");
				THROW_WINAPI(L"Cannot get Raw Input Data!");
			}

			unsigned char* bytes = new unsigned char[sizeInBytes];

			ret = GetRawInputData(hRawInput, RID_INPUT, bytes, &sizeInBytes, sizeof(RAWINPUTHEADER));
			if (ret != sizeInBytes)
			{
				Log::LogMessage(L"Cannot get Raw Input Data!");
				THROW_WINAPI(L"Cannot get Raw Input Data!");
			}

			RAWINPUT* rawDataInput = reinterpret_cast<RAWINPUT*>(bytes);

			if (rawDataInput->header.dwType == RIM_TYPEMOUSE)
			{
				// First of all, try Relative Mouse Position

				int lastX;
				int lastY;

				if (rawDataInput->data.mouse.usFlags == MOUSE_MOVE_RELATIVE)
				{
					lastX = rawDataInput->data.mouse.lLastX;
					lastY = rawDataInput->data.mouse.lLastY;

					// std::wstringstream wsstream;
					// wsstream << L"x: " << lastX << L", y: " << lastY << L"; ";
					// LogableData dataToLog{ L"InputSystem", wsstream.str(), LogLevel::LOG_DEBUG };
					// Log::LogMessage(dataToLog);
				}

				InputSystem::OnMouseRawDeltaMove(lastX, lastY);
			}

			if (bytes != nullptr)
				delete bytes;

			*/
		}
		break;

		case WM_ENTERSIZEMOVE:
		{
			/*
			windowPtr->SetResizingState(true);
			*/
		}
		break;
		case WM_EXITSIZEMOVE:
		{
			/*
			if (windowPtr->IsResizing())
			{
				Application::GetAppInstance()->OnResize();
				windowPtr->SetResizingState(false);
			}
			*/
		}
		break;
		case WM_SIZE:
		{
			/*
			windowPtr->SetWindowWidth(LOWORD(lParam));
			windowPtr->SetWindowHeight(HIWORD(lParam));

			// Workaround in order to handle Alt-Enter fullscreen switching
			if (!windowPtr->IsResizing())
			{
				Application::GetAppInstance()->OnResize();
			}
			*/
		}
		break;

		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);

			FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

			EndPaint(hWnd, &ps);
			// DefWindowProcW(hWnd, uMsg, wParam, lParam);
		}
		return 0;

		case WM_CLOSE:
		{
			if (MessageBoxA(hWnd, "Do you really want to quit?", windowSettings.windowTitle.c_str(), MB_OKCANCEL) == IDOK)
			{
				// return DestroyWindow(hWnd);

				WindowCloseCallbackData windowCloseCallbackData;
				windowCloseCallbackData.closeState = true;

				assert(windowPtr && "Window pointer cannot be 'nullptr'!");

				// windowCallback->eventRegistry->NotifyCallbackEvent(keyCallbackData);
				windowPtr->eventRegistry->NotifyCallbackEventDelayed(windowCloseCallbackData);
			}
		}
		return 0;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		return 0;
		}

		return DefWindowProcA(hWnd, uMsg, wParam, lParam);
	}
}