#pragma once

#include "InputCommon.h"

#include <bitset>
#include <unordered_map>

namespace dxe
{
	// Mouse

	class Mouse : public InputDevice
	{
	public:

		enum class KeyCode
		{
			DXE_KEY_UNIDENTIFIED = -1,

			DXE_LEFT_MOUSE_BUTTON = 0,	 // LMB key
			DXE_MIDDLE_MOUSE_BUTTON = 1, // MMB key
			DXE_RIGHT_MOUSE_BUTTON = 2,  // RMB key

			COUNT = 3,
			MAX_CODE = DXE_RIGHT_MOUSE_BUTTON,
		};

		Mouse();

		InputDeviceType GetInputDeviceType() override;

		void SetButtonState(Mouse::KeyCode keycode, KeyActionType action);
		bool ButtonPressed(Mouse::KeyCode keycode) const;

		void ClearButtonsState();

		std::string GetKeyName(Mouse::KeyCode keycode) const;

	private:

		void InitializeKeyNamesMap();

		std::bitset<static_cast<size_t>(Mouse::KeyCode::MAX_CODE) + 1> keysState;
		std::unordered_map<Mouse::KeyCode, std::string> keyNames;
	};

	// MouseCombo

	class MouseCombo : public KeyCombo
	{
	public:

		MouseCombo(Mouse::KeyCode keycode,
			bool ctrlActive, bool shiftActive, bool altActive);

		KeyComboType GetKeyComboType() override;

		uint16_t GetComboId() const;

		Mouse::KeyCode GetMouseKeyCode() const;

		bool CtrlModifierActive() const;
		bool ShiftModifierActive() const;
		bool AltModifierActive() const;

	private:

		void SetComboId();

		Mouse::KeyCode keycode{ Mouse::KeyCode::DXE_KEY_UNIDENTIFIED };
		uint16_t comboId{ 0 };

		bool ctrlActive{ false };
		bool shiftActive{ false };
		bool altActive{ false };
	};
}