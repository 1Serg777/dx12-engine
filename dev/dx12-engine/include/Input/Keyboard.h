#pragma once

#include "InputCommon.h"

#include <bitset>
#include <string>
#include <unordered_map>

namespace dxe
{
	// Keyboard

	class Keyboard : public InputDevice
	{
	public:

		enum class KeyCode
		{
			DXE_KEY_UNIDENTIFIED = -1,

			DXE_KEY_ESC = 0x29, // ESC key
			DXE_KEY_DEL = 0x2A, // DEL key

			DXE_KEY_0 = 0x30, // 0 key
			DXE_KEY_1 = 0x31, // 1 key
			DXE_KEY_2 = 0x32, // 2 key
			DXE_KEY_3 = 0x33, // 3 key
			DXE_KEY_4 = 0x34, // 4 key
			DXE_KEY_5 = 0x35, // 5 key
			DXE_KEY_6 = 0x36, // 6 key
			DXE_KEY_7 = 0x37, // 7 key
			DXE_KEY_8 = 0x38, // 8 key
			DXE_KEY_9 = 0x39, // 9 key

			DXE_KEY_A = 0x41, // A key
			DXE_KEY_B = 0x42, // B key
			DXE_KEY_C = 0x43, // C key
			DXE_KEY_D = 0x44, // D key
			DXE_KEY_E = 0x45, // E key
			DXE_KEY_F = 0x46, // F key
			DXE_KEY_G = 0x47, // G key
			DXE_KEY_H = 0x48, // H key
			DXE_KEY_I = 0x49, // I key
			DXE_KEY_J = 0x4A, // J key
			DXE_KEY_K = 0x4B, // K key
			DXE_KEY_L = 0x4C, // L key
			DXE_KEY_M = 0x4D, // M key
			DXE_KEY_N = 0x4E, // N key
			DXE_KEY_O = 0x4F, // O key
			DXE_KEY_P = 0x50, // P key
			DXE_KEY_Q = 0x51, // Q key
			DXE_KEY_R = 0x52, // R key
			DXE_KEY_S = 0x53, // S key
			DXE_KEY_T = 0x54, // T key
			DXE_KEY_U = 0x55, // U key
			DXE_KEY_V = 0x56, // V key
			DXE_KEY_W = 0x57, // W key
			DXE_KEY_X = 0x58, // X key
			DXE_KEY_Y = 0x59, // Y key
			DXE_KEY_Z = 0x5A, // Z key

			DXE_KEY_F1 = 0x74, // F1 key
			DXE_KEY_F2 = 0x75, // F2 key
			DXE_KEY_F3 = 0x76, // F3 key
			DXE_KEY_F4 = 0x77, // F4 key
			DXE_KEY_F5 = 0x78, // F5 key
			DXE_KEY_F6 = 0x79, // F6 key
			DXE_KEY_F7 = 0x7A, // F7 key
			DXE_KEY_F8 = 0x7B, // F8 key
			DXE_KEY_F9 = 0x7C, // F9 key
			DXE_KEY_F10 = 0x7D, // F10 key
			DXE_KEY_F11 = 0x7E, // F11 key
			DXE_KEY_F12 = 0x7F, // F12 key

			DXE_KEY_LEFT_CTRL = 0xE0,  // LCTRL  key
			DXE_KEY_LEFT_SHIFT = 0xE1, // LSHIFT key
			DXE_KEY_LEFT_ALT = 0xE2,   // LALT   key

			DXE_KEY_RIGHT_CTRL = 0xE4,  // RCTRL  key
			DXE_KEY_RIGHT_SHIFT = 0xE5, // RSHIFT key
			DXE_KEY_RIGHT_ALT = 0xE6,   // RALT   key

			COUNT = 56,
			MAX_CODE = DXE_KEY_RIGHT_ALT,
		};

		Keyboard();

		InputDeviceType GetInputDeviceType() override;

		void SetButtonState(Keyboard::KeyCode keycode, KeyActionType action);
		bool ButtonPressed(Keyboard::KeyCode keycode) const;

		bool IsModifierKey(Keyboard::KeyCode keycode) const;

		bool CtrlModifierPressed() const;
		bool ShiftModifierPressed() const;
		bool AltModifierPressed() const;

		void ClearButtonsState();

		std::string GetKeyName(Keyboard::KeyCode keycode) const;

	private:

		void InitializeKeyNamesMap();

		std::bitset<static_cast<size_t>(Keyboard::KeyCode::MAX_CODE) + 1> keysState;
		std::unordered_map<Keyboard::KeyCode, std::string> keyNames;
	};

	// KeyboardCombo

	class KeyboardCombo : public KeyCombo
	{
	public:

		KeyboardCombo(Keyboard::KeyCode keycode,
			bool ctrlActive, bool shiftActive, bool altActive);

		KeyComboType GetKeyComboType() override;

		uint16_t GetComboId() const;

		Keyboard::KeyCode GetKeyboardKeyCode() const;

		bool CtrlModifierActive() const;
		bool ShiftModifierActive() const;
		bool AltModifierActive() const;

	private:

		void SetComboId();

		Keyboard::KeyCode keycode{ Keyboard::KeyCode::DXE_KEY_UNIDENTIFIED };
		uint16_t comboId{ 0 };

		bool ctrlActive{ false };
		bool shiftActive{ false };
		bool altActive{ false };
	};
}