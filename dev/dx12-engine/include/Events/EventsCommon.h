#pragma once

#include "Input/InputCommon.h"
#include "Input/Keyboard.h"
#include "Input/Mouse.h"

#include <functional>

namespace dxe
{
	enum class CallbackType
	{
		UNIDENTIFIED = -1,

		KEYBOARD_KEY = 0,

		MOUSE_BUTTON = 1,
		MOUSE_SCROLL = 2,
		MOUSE_POS = 3,

		FRAMEBUFFER_RESIZE = 4,

		WINDOW_RESIZE = 5,
		WINDOW_CLOSE = 6,

		CRITICAL_ASSETS_LOADED = 7,

		COUNT = 8
	};

	class CallbackData
	{
	public:

		CallbackData(CallbackType type)
			: callbackType(type) {}

		CallbackType GetCallbackType() const
		{
			return callbackType;
		}

	protected:

		CallbackType callbackType{ CallbackType::UNIDENTIFIED };
	};

	class KeyboardKeyCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::KEYBOARD_KEY;
		using Callable = std::function<void(const KeyboardKeyCallbackData&)>;

		KeyboardKeyCallbackData()
			: CallbackData(CallbackType::KEYBOARD_KEY) {}

		Keyboard::KeyCode key{ Keyboard::KeyCode::DXE_KEY_UNIDENTIFIED };
		KeyActionType action{ KeyActionType::UNIDENTIFIED };

		int scancode{ 0 };
	};

	class MouseButtonCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::MOUSE_BUTTON;
		using Callable = std::function<void(const MouseButtonCallbackData&)>;

		MouseButtonCallbackData()
			: CallbackData(CallbackType::MOUSE_BUTTON) {}

		Mouse::KeyCode key{ Mouse::KeyCode::DXE_KEY_UNIDENTIFIED };
		KeyActionType action{ KeyActionType::UNIDENTIFIED };
	};

	class MouseScrollCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::MOUSE_SCROLL;
		using Callable = std::function<void(const MouseScrollCallbackData&)>;

		MouseScrollCallbackData()
			: CallbackData(CallbackType::MOUSE_SCROLL) {}

		double scrollDelta{ 0.0 };
	};

	class MousePosCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::MOUSE_POS;
		using Callable = std::function<void(const MousePosCallbackData&)>;

		MousePosCallbackData()
			: CallbackData(CallbackType::MOUSE_POS) {}

		double xpos{ 0.0 };
		double ypos{ 0.0 };
	};

	class WindowResizeCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::WINDOW_RESIZE;
		using Callable = std::function<void(const WindowResizeCallbackData&)>;

		WindowResizeCallbackData()
			: CallbackData(CallbackType::WINDOW_RESIZE) {}

		uint32_t width{ 0 };
		uint32_t height{ 0 };
	};

	class FramebufferResizeCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::FRAMEBUFFER_RESIZE;
		using Callable = std::function<void(const FramebufferResizeCallbackData&)>;

		FramebufferResizeCallbackData()
			: CallbackData(CallbackType::FRAMEBUFFER_RESIZE) {}

		uint32_t width{ 0 };
		uint32_t height{ 0 };
	};

	class WindowCloseCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::WINDOW_CLOSE;
		using Callable = std::function<void(const WindowCloseCallbackData&)>;

		WindowCloseCallbackData()
			: CallbackData(CallbackType::WINDOW_CLOSE) {}

		bool closeState{ false };
	};

	class CriticalAssetsLoadedCallbackData : public CallbackData
	{
	public:

		static constexpr CallbackType Type = CallbackType::CRITICAL_ASSETS_LOADED;
		using Callable = std::function<void(const CriticalAssetsLoadedCallbackData&)>;

		CriticalAssetsLoadedCallbackData()
			: CallbackData(CallbackType::CRITICAL_ASSETS_LOADED) {}
	};

	// Callback

	class CallbackBase
	{
	public:

		using CallbackId = int;

		CallbackBase()
			: id(GenerateUniqueId()) {}

		CallbackId GetCallbackId() const
		{
			return id;
		}

	protected:

		CallbackId GenerateUniqueId()
		{
			static CallbackId uniqueId{ 0 };
			return uniqueId++;
		}

	private:

		CallbackId id{ 0 };
	};

	template <typename CallbackData>
	class Callback : public CallbackBase
	{
	public:

		Callback(typename CallbackData::Callable callable)
			: callable(callable) {}

		void Invoke(const CallbackData& callbackData)
		{
			callable(callbackData);
		}

	private:

		typename CallbackData::Callable callable;
	};

	template<typename CallbackData>
	inline bool operator==(const Callback<CallbackData>& c1, const Callback<CallbackData>& c2)
	{
		return c1.GetCallbackId() == c2.GetCallbackId();
	}
	template<typename CallbackData>
	inline bool operator!=(const Callback<CallbackData>& c1, const Callback<CallbackData>& c2)
	{
		return !(c1 == c2);
	}
}