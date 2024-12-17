#include "Input/Mouse.h"

namespace dxe
{
	// Mouse

	Mouse::Mouse()
	{
		InitializeKeyNamesMap();
	}

	InputDeviceType Mouse::GetInputDeviceType()
	{
		return InputDeviceType::MOUSE;
	}

	void Mouse::SetButtonState(Mouse::KeyCode keycode, KeyActionType action)
	{
		keysState.set(static_cast<size_t>(keycode), (action == KeyActionType::PRESSED ? true : false));
	}
	bool Mouse::ButtonPressed(Mouse::KeyCode keycode) const
	{
		return keysState.test(static_cast<size_t>(keycode));
	}

	void Mouse::ClearButtonsState()
	{
		keysState.reset();
	}

	std::string Mouse::GetKeyName(Mouse::KeyCode keycode) const
	{
		auto search = keyNames.find(keycode);
		if (search == keyNames.end())
		{
			return "UNIDENTIFIED";
		}
		return search->second;
	}

	void Mouse::InitializeKeyNamesMap()
	{
		keyNames.insert({ KeyCode::DXE_LEFT_MOUSE_BUTTON,   "DXE_LEFT_MOUSE_BUTTON" });
		keyNames.insert({ KeyCode::DXE_MIDDLE_MOUSE_BUTTON, "DXE_MIDDLE_MOUSE_BUTTON" });
		keyNames.insert({ KeyCode::DXE_RIGHT_MOUSE_BUTTON,  "DXE_RIGHT_MOUSE_BUTTON" });
	}

	// MouseCombo

	MouseCombo::MouseCombo(Mouse::KeyCode keycode,
		bool ctrlActive, bool shiftActive, bool altActive)
		: ctrlActive(ctrlActive), shiftActive(shiftActive), altActive(altActive),
		keycode(keycode)
	{
		SetComboId();
	}

	KeyComboType MouseCombo::GetKeyComboType()
	{
		return KeyComboType::MOUSE;
	}

	uint16_t MouseCombo::GetComboId() const
	{
		return comboId;
	}

	Mouse::KeyCode MouseCombo::GetMouseKeyCode() const
	{
		return keycode;
	}

	bool MouseCombo::CtrlModifierActive() const
	{
		return ctrlActive;
	}
	bool MouseCombo::ShiftModifierActive() const
	{
		return shiftActive;
	}
	bool MouseCombo::AltModifierActive() const
	{
		return altActive;
	}

	void MouseCombo::SetComboId()
	{
		uint16_t highByte = static_cast<uint16_t>(keycode) << 8;
		uint16_t lowByte =
			(ctrlActive ? 1 : 0) |
			(shiftActive ? 1 << 1 : 0) |
			(altActive ? 1 << 2 : 0);

		comboId = highByte | lowByte;
	}
}