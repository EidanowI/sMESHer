#pragma once
#include <Windows.h>

#include "Windows.h"

#include <bitset>
#include <vector>



class InputSystem {
	friend class AppWindow;
	friend class Camera;
	friend class InputActionFunctions;
	friend LRESULT WindowProcess(HWND, UINT, WPARAM, LPARAM);

	friend void PressRightButton(WPARAM wParam, int x, int y) noexcept;
	friend void ReleaseRightButton(WPARAM wParam, int x, int y) noexcept;

public:
	enum MouseButton {
		LEFT_MOUSE_BUTON = 0,
		RIGHT_MOUSE_BUTTON = 1,
		MIDDLE_MOUSE_BUTTON = 2,
		XBUTTON1_MOUSE_BUTTON = 3,
		XBUTTON2_MOUSE_BUTTON = 4,


		THIS_USED_ONLY_FOR_SETTING_SIZE
	};

private:
	struct MousePressedInfo {
		WPARAM wParam;
		int x;
		int y;
	};

public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;
	static void PopulateWithStandarts() noexcept;

	static void UpdateInput() noexcept;

	static void AddPressedKey(unsigned char keyCode, LPARAM lParam) noexcept;
	static void RemovePressedKey(unsigned char keyCode) noexcept;

	static void AddPressedMButton(unsigned char mButton, WPARAM wParam, LPARAM lParam) noexcept;
	static void RemovePressedMButton(unsigned char mButton) noexcept;

private:
	static std::vector<unsigned char> s_rawBuffer;

	static void (**s_ppFunctions_KEY_ONCE)(LPARAM lParam) noexcept;
	static void (**s_ppFunctions_KEY_IS_PRESSED)(LPARAM lParam) noexcept;
	static void (**s_ppFunctions_KEY_RELEASE)(LPARAM lParam) noexcept;

	static void (**s_ppFunctions_MOUSE_BUTTON_ONCE)(WPARAM wParam, int x, int y) noexcept;
	static void (**s_ppFunctions_MOUSE_BUTTON_IS_PRESSED)(WPARAM wParam, int x, int y) noexcept;
	static void (**s_ppFunctions_MOUSE_BUTTON_RELEASE)(WPARAM wParam, int x, int y) noexcept;

	static void (*s_pFunction_MOUSE_WHEEL_STEP_UP)(int x, int y) noexcept;
	static void (*s_pFunction_MOUSE_WHEEL_STEP_DOWN)(int x, int y) noexcept;

	static unsigned char* s_pPressedKeyCodes;
	static unsigned short s_pressedKeyCount;
	static unsigned char* s_pFromKeyCodesToIndex;
	static std::bitset<256> s_isKeysPressed;
	static LPARAM* s_pKeysLParams;

	static unsigned char* s_pPressedMButtons;
	static unsigned char s_pressedMButtonsCount;
	static unsigned char* s_pFromMButtonsToIndex;
	static std::bitset<THIS_USED_ONLY_FOR_SETTING_SIZE> s_isMouseBPressed;
	static MousePressedInfo* s_pMButtonsInfos;

	static bool s_isCursorReceivesDelta;
	static int s_currentCursorPosX;
	static int s_currentCursorPosY;
	static int s_cursorDeltaX;
	static int s_cursorDeltaY;
	static bool s_isInputDelta;

	static int s_wheelDelta;
};
