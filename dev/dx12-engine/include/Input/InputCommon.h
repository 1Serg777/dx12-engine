#pragma once

namespace dxe
{
	// Input devices

	enum class InputDeviceType
	{
		UNIDENTIFIED = -1,

		MOUSE,
		KEYBOARD,
		GAMEPAD // not supported yet!
	};

	class InputDevice
	{
	public:
		virtual InputDeviceType GetInputDeviceType() = 0;
	};

	enum class MouseMovementType
	{
		SCREEN_COORDS,
		RAW_DELTA
	};

	// Actions and combos

	enum class KeyActionType
	{
		UNIDENTIFIED = -1,

		PRESSED,
		REPEATED,
		RELEASED,
	};

	enum KeyComboType
	{
		MOUSE,
		KEYBOARD,
		GAMEPAD // isn't supported yet!
	};

	class KeyCombo
	{
	public:
		virtual KeyComboType GetKeyComboType() = 0;
	};
}