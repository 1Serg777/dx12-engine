#include "Input/Keyboard.h"

namespace dxe
{
	// Keyboard

	Keyboard::Keyboard()
	{
		InitializeKeyNamesMap();
	}

	InputDeviceType Keyboard::GetInputDeviceType()
	{
		return InputDeviceType::KEYBOARD;
	}

	void Keyboard::SetButtonState(Keyboard::KeyCode keycode, KeyActionType action)
	{
		if (action == KeyActionType::PRESSED || action == KeyActionType::REPEATED)
			keysState.set(static_cast<size_t>(keycode), true);
		else if (action == KeyActionType::RELEASED)
			keysState.set(static_cast<size_t>(keycode), false);
	}
	bool Keyboard::ButtonPressed(Keyboard::KeyCode keycode) const
	{
		return keysState.test(static_cast<size_t>(keycode));
	}

	bool Keyboard::IsModifierKey(Keyboard::KeyCode keycode) const
	{
		return
			(keycode == Keyboard::KeyCode::DXE_KEY_LEFT_CTRL) ||
			(keycode == Keyboard::KeyCode::DXE_KEY_LEFT_SHIFT) ||
			(keycode == Keyboard::KeyCode::DXE_KEY_LEFT_ALT) ||
			(keycode == Keyboard::KeyCode::DXE_KEY_RIGHT_CTRL) ||
			(keycode == Keyboard::KeyCode::DXE_KEY_RIGHT_SHIFT) ||
			(keycode == Keyboard::KeyCode::DXE_KEY_RIGHT_ALT);
	}

	bool Keyboard::CtrlModifierPressed() const
	{
		return
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_LEFT_CTRL) ||
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_RIGHT_CTRL);
	}
	bool Keyboard::ShiftModifierPressed() const
	{
		return
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_LEFT_SHIFT) ||
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_RIGHT_SHIFT);
	}
	bool Keyboard::AltModifierPressed() const
	{
		return
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_LEFT_ALT) ||
			ButtonPressed(Keyboard::KeyCode::DXE_KEY_RIGHT_ALT);
	}

	void Keyboard::ClearButtonsState()
	{
		keysState.reset();
	}

	std::string Keyboard::GetKeyName(Keyboard::KeyCode keycode) const
	{
		auto search = keyNames.find(keycode);
		if (search == keyNames.end())
		{
			return "UNIDENTIFIED";
		}
		return search->second;
	}

	void Keyboard::InitializeKeyNamesMap()
	{
		keyNames.insert({ KeyCode::DXE_KEY_ESC, "DXE_KEY_ESC" });
		keyNames.insert({ KeyCode::DXE_KEY_DEL, "DXE_KEY_DEL" });

		keyNames.insert({ KeyCode::DXE_KEY_0, "DXE_KEY_0" });
		keyNames.insert({ KeyCode::DXE_KEY_1, "DXE_KEY_1" });
		keyNames.insert({ KeyCode::DXE_KEY_2, "DXE_KEY_2" });
		keyNames.insert({ KeyCode::DXE_KEY_3, "DXE_KEY_3" });
		keyNames.insert({ KeyCode::DXE_KEY_4, "DXE_KEY_4" });
		keyNames.insert({ KeyCode::DXE_KEY_5, "DXE_KEY_5" });
		keyNames.insert({ KeyCode::DXE_KEY_6, "DXE_KEY_6" });
		keyNames.insert({ KeyCode::DXE_KEY_7, "DXE_KEY_7" });
		keyNames.insert({ KeyCode::DXE_KEY_8, "DXE_KEY_8" });
		keyNames.insert({ KeyCode::DXE_KEY_9, "DXE_KEY_9" });

		keyNames.insert({ KeyCode::DXE_KEY_A, "DXE_KEY_A" });
		keyNames.insert({ KeyCode::DXE_KEY_B, "DXE_KEY_B" });
		keyNames.insert({ KeyCode::DXE_KEY_C, "DXE_KEY_C" });
		keyNames.insert({ KeyCode::DXE_KEY_D, "DXE_KEY_D" });
		keyNames.insert({ KeyCode::DXE_KEY_E, "DXE_KEY_E" });
		keyNames.insert({ KeyCode::DXE_KEY_F, "DXE_KEY_F" });
		keyNames.insert({ KeyCode::DXE_KEY_G, "DXE_KEY_G" });
		keyNames.insert({ KeyCode::DXE_KEY_H, "DXE_KEY_H" });
		keyNames.insert({ KeyCode::DXE_KEY_I, "DXE_KEY_I" });
		keyNames.insert({ KeyCode::DXE_KEY_J, "DXE_KEY_J" });
		keyNames.insert({ KeyCode::DXE_KEY_K, "DXE_KEY_K" });
		keyNames.insert({ KeyCode::DXE_KEY_L, "DXE_KEY_L" });
		keyNames.insert({ KeyCode::DXE_KEY_M, "DXE_KEY_M" });
		keyNames.insert({ KeyCode::DXE_KEY_N, "DXE_KEY_N" });
		keyNames.insert({ KeyCode::DXE_KEY_O, "DXE_KEY_O" });
		keyNames.insert({ KeyCode::DXE_KEY_P, "DXE_KEY_P" });
		keyNames.insert({ KeyCode::DXE_KEY_Q, "DXE_KEY_Q" });
		keyNames.insert({ KeyCode::DXE_KEY_R, "DXE_KEY_R" });
		keyNames.insert({ KeyCode::DXE_KEY_S, "DXE_KEY_S" });
		keyNames.insert({ KeyCode::DXE_KEY_T, "DXE_KEY_T" });
		keyNames.insert({ KeyCode::DXE_KEY_U, "DXE_KEY_U" });
		keyNames.insert({ KeyCode::DXE_KEY_V, "DXE_KEY_V" });
		keyNames.insert({ KeyCode::DXE_KEY_W, "DXE_KEY_W" });
		keyNames.insert({ KeyCode::DXE_KEY_X, "DXE_KEY_X" });
		keyNames.insert({ KeyCode::DXE_KEY_Y, "DXE_KEY_Y" });
		keyNames.insert({ KeyCode::DXE_KEY_Z, "DXE_KEY_Z" });

		keyNames.insert({ KeyCode::DXE_KEY_LEFT_CTRL,  "DXE_KEY_LEFT_CTRL" });
		keyNames.insert({ KeyCode::DXE_KEY_LEFT_SHIFT, "DXE_KEY_LEFT_SHIFT" });
		keyNames.insert({ KeyCode::DXE_KEY_LEFT_ALT,   "DXE_KEY_LEFT_ALT" });

		keyNames.insert({ KeyCode::DXE_KEY_RIGHT_CTRL,  "DXE_KEY_RIGHT_CTRL" });
		keyNames.insert({ KeyCode::DXE_KEY_RIGHT_SHIFT, "DXE_KEY_RIGHT_SHIFT" });
		keyNames.insert({ KeyCode::DXE_KEY_RIGHT_ALT,   "DXE_KEY_RIGHT_ALT" });
	}

	// KeyboardCombo

	KeyboardCombo::KeyboardCombo(Keyboard::KeyCode keycode,
		bool ctrlActive, bool shiftActive, bool altActive)
		: ctrlActive(ctrlActive), shiftActive(shiftActive), altActive(altActive),
		keycode(keycode)
	{
		SetComboId();
	}

	KeyComboType KeyboardCombo::GetKeyComboType()
	{
		return KeyComboType::KEYBOARD;
	}

	uint16_t KeyboardCombo::GetComboId() const
	{
		return comboId;
	}

	Keyboard::KeyCode KeyboardCombo::GetKeyboardKeyCode() const
	{
		return keycode;
	}

	bool KeyboardCombo::CtrlModifierActive() const
	{
		return ctrlActive;
	}
	bool KeyboardCombo::ShiftModifierActive() const
	{
		return shiftActive;
	}
	bool KeyboardCombo::AltModifierActive() const
	{
		return altActive;
	}

	void KeyboardCombo::SetComboId()
	{
		uint16_t highByte = static_cast<uint16_t>(keycode) << 8;
		uint16_t lowByte =
			(ctrlActive ? 1 : 0) |
			(shiftActive ? 1 << 1 : 0) |
			(altActive ? 1 << 2 : 0);

		comboId = highByte | lowByte;
	}
}