#include "InputSystem.h"
#include "../AppWindow/AppWindow.h"
#include "../GlobalDefs.h"
#include "../Renderer/Renderer.h"
#include "../ImGuiManager/ImGUIManager.h"
#include "InputActions.h"



std::vector<unsigned char> InputSystem::s_rawBuffer = std::vector<unsigned char>();

void (**InputSystem::s_ppFunctions_KEY_ONCE)(LPARAM lParam) noexcept = nullptr;
void (**InputSystem::s_ppFunctions_KEY_IS_PRESSED)(LPARAM lParam) noexcept = nullptr;
void (**InputSystem::s_ppFunctions_KEY_RELEASE)(LPARAM lParam) noexcept = nullptr;

void (**InputSystem::s_ppFunctions_MOUSE_BUTTON_ONCE)(WPARAM wParam, int x, int y) noexcept = nullptr;
void (**InputSystem::s_ppFunctions_MOUSE_BUTTON_IS_PRESSED)(WPARAM wParam, int x, int y) noexcept;
void (**InputSystem::s_ppFunctions_MOUSE_BUTTON_RELEASE)(WPARAM wParam, int x, int y) noexcept;

void (*InputSystem::s_pFunction_MOUSE_WHEEL_STEP_UP)(int x, int y) noexcept = nullptr;
void (*InputSystem::s_pFunction_MOUSE_WHEEL_STEP_DOWN)(int x, int y) noexcept = nullptr;

unsigned char* InputSystem::s_pPressedKeyCodes = nullptr;
unsigned short InputSystem::s_pressedKeyCount = 0u;
unsigned char* InputSystem::s_pFromKeyCodesToIndex = nullptr;
std::bitset<256> InputSystem::s_isKeysPressed = std::bitset<256>();
LPARAM* InputSystem::s_pKeysLParams = nullptr;

unsigned char* InputSystem::s_pPressedMButtons = nullptr;
unsigned char InputSystem::s_pressedMButtonsCount = 0u;
unsigned char* InputSystem::s_pFromMButtonsToIndex = nullptr;
std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE> InputSystem::s_isMouseBPressed = std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE>();
InputSystem::MousePressedInfo* InputSystem::s_pMButtonsInfos = nullptr;

bool InputSystem::s_isCursorReceivesDelta = true;
int InputSystem::s_currentCursorPosX = 0;
int InputSystem::s_currentCursorPosY = 0;
int InputSystem::s_cursorDeltaX = 0;
int InputSystem::s_cursorDeltaY = 0;
bool InputSystem::s_isInputDelta = false;

int InputSystem::s_wheelDelta = 0;


void InputSystem::Initialize() noexcept {
	s_pPressedKeyCodes = new unsigned char[256];
	s_pFromKeyCodesToIndex = new unsigned char[256];
	s_pKeysLParams = new LPARAM[256];

	s_pPressedMButtons = new unsigned char[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];
	s_pFromMButtonsToIndex = new unsigned char[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];
	s_pMButtonsInfos = new InputSystem::MousePressedInfo[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE];


	s_ppFunctions_KEY_ONCE = new (void (*[256])(LPARAM lParam)noexcept);
	s_ppFunctions_KEY_IS_PRESSED = new (void (*[256])(LPARAM lParam)noexcept);
	s_ppFunctions_KEY_RELEASE = new (void (*[256])(LPARAM lParam)noexcept);
	for (int i = 0; i < 256; i++) {
		s_ppFunctions_KEY_ONCE[i] = InputActionFunctions::EMPTYFUNC;
		s_ppFunctions_KEY_IS_PRESSED[i] = InputActionFunctions::EMPTYFUNC;
		s_ppFunctions_KEY_RELEASE[i] = InputActionFunctions::EMPTYFUNC;
	}

	s_ppFunctions_MOUSE_BUTTON_ONCE = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);
	s_ppFunctions_MOUSE_BUTTON_IS_PRESSED = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);
	s_ppFunctions_MOUSE_BUTTON_RELEASE = new (void(*[MouseButton::THIS_USED_ONLY_FOR_SETTING_SIZE])(WPARAM wParam, int x, int y) noexcept);

	for (int i = 0; i < THIS_USED_ONLY_FOR_SETTING_SIZE; i++) {
		s_ppFunctions_MOUSE_BUTTON_ONCE[i] = InputActionFunctions::EMPTYFUNC;
		s_ppFunctions_MOUSE_BUTTON_IS_PRESSED[i] = InputActionFunctions::EMPTYFUNC;
		s_ppFunctions_MOUSE_BUTTON_RELEASE[i] = InputActionFunctions::EMPTYFUNC;
	}

	s_pFunction_MOUSE_WHEEL_STEP_UP = InputActionFunctions::EMPTYFUNC;
	s_pFunction_MOUSE_WHEEL_STEP_DOWN = InputActionFunctions::EMPTYFUNC;
	

	InputSystem::s_isCursorReceivesDelta = false;
}

void InputSystem::Terminate() noexcept {
	if (s_pPressedKeyCodes != nullptr) return;

	delete[] s_pPressedKeyCodes; s_pPressedKeyCodes = nullptr;
	s_pressedKeyCount = 0u;
	delete[] s_pFromKeyCodesToIndex; s_pFromKeyCodesToIndex = nullptr;
	s_isKeysPressed = std::bitset<256>();
	delete[] s_pKeysLParams; s_pKeysLParams = nullptr;


	delete[] s_pPressedMButtons; s_pPressedMButtons = nullptr;
	s_pressedMButtonsCount = 0u;
	delete[] s_pFromMButtonsToIndex; s_pFromMButtonsToIndex = nullptr;
	s_isMouseBPressed = std::bitset<InputSystem::THIS_USED_ONLY_FOR_SETTING_SIZE>();
	delete[] s_pMButtonsInfos; s_pMButtonsInfos = nullptr;

	s_currentCursorPosX = 0;
	s_currentCursorPosY = 0;
	s_cursorDeltaX = 0;
	s_cursorDeltaY = 0;

	s_wheelDelta = 0;

	delete[] s_ppFunctions_KEY_ONCE;
	delete[] s_ppFunctions_KEY_IS_PRESSED;
	delete[] s_ppFunctions_KEY_RELEASE;
	s_ppFunctions_KEY_ONCE = nullptr;
	s_ppFunctions_KEY_IS_PRESSED = nullptr;
	s_ppFunctions_KEY_RELEASE = nullptr;

	delete[] s_ppFunctions_MOUSE_BUTTON_ONCE;
	delete[] s_ppFunctions_MOUSE_BUTTON_IS_PRESSED;
	delete[] s_ppFunctions_MOUSE_BUTTON_RELEASE;
	s_ppFunctions_MOUSE_BUTTON_ONCE = nullptr;
	s_ppFunctions_MOUSE_BUTTON_IS_PRESSED = nullptr;
	s_ppFunctions_MOUSE_BUTTON_RELEASE = nullptr;

	s_pFunction_MOUSE_WHEEL_STEP_UP = nullptr;
	s_pFunction_MOUSE_WHEEL_STEP_DOWN = nullptr;
}

void InputSystem::PopulateWithStandarts() noexcept {
	s_ppFunctions_MOUSE_BUTTON_ONCE[MouseButton::RIGHT_MOUSE_BUTTON] = InputActionFunctions::PressRightButton;
	s_ppFunctions_MOUSE_BUTTON_RELEASE[MouseButton::RIGHT_MOUSE_BUTTON] = InputActionFunctions::ReleaseRightButton;

	InputSystem::s_ppFunctions_KEY_IS_PRESSED['D'] = InputActionFunctions::Move_Right;
	InputSystem::s_ppFunctions_KEY_IS_PRESSED['A'] = InputActionFunctions::Move_Left;
	InputSystem::s_ppFunctions_KEY_IS_PRESSED['W'] = InputActionFunctions::Move_Forward;
	InputSystem::s_ppFunctions_KEY_IS_PRESSED['S'] = InputActionFunctions::Move_Back;
	InputSystem::s_ppFunctions_KEY_IS_PRESSED[VK_SPACE] = InputActionFunctions::Move_Up;
	InputSystem::s_ppFunctions_KEY_IS_PRESSED[VK_SHIFT] = InputActionFunctions::Move_Down;

	InputSystem::s_pFunction_MOUSE_WHEEL_STEP_UP = InputActionFunctions::Move_Forward_By_Wheel;
	InputSystem::s_pFunction_MOUSE_WHEEL_STEP_DOWN = InputActionFunctions::Move_Back_By_Wheel;
}


void InputSystem::UpdateInput() noexcept {
	for (int i = 0; i < s_pressedKeyCount; i++) {
		InputSystem::s_ppFunctions_KEY_IS_PRESSED[s_pPressedKeyCodes[i]](s_pKeysLParams[s_pPressedKeyCodes[i]]);
	}

	for (int i = 0; i < s_pressedMButtonsCount; i++) {
		InputSystem::MousePressedInfo info = s_pMButtonsInfos[s_pPressedMButtons[i]];
		InputSystem::s_ppFunctions_MOUSE_BUTTON_IS_PRESSED[s_pPressedMButtons[i]](info.wParam, info.x, info.y);
	}

	s_cursorDeltaX = 0;
	s_cursorDeltaY = 0;
}

void InputSystem::AddPressedKey(unsigned char keyCode, LPARAM lParam) noexcept {
	if (s_isKeysPressed[keyCode] == 0b1) return;

	s_isKeysPressed[keyCode] = 0b1;
	s_pKeysLParams[keyCode] = lParam;

	s_pPressedKeyCodes[s_pressedKeyCount] = keyCode;
	s_pFromKeyCodesToIndex[keyCode] = s_pressedKeyCount;
	s_pressedKeyCount++;
}

void InputSystem::RemovePressedKey(unsigned char keyCode) noexcept {
	if (s_isKeysPressed[keyCode] == 0b0) return;

	s_isKeysPressed[keyCode] = false;
	s_pKeysLParams[keyCode] = 0;

	unsigned char tmp = s_pFromKeyCodesToIndex[keyCode];
	s_pPressedKeyCodes[tmp] = s_pPressedKeyCodes[s_pressedKeyCount - 1];
	s_pFromKeyCodesToIndex[s_pPressedKeyCodes[tmp]] = tmp;
	s_pressedKeyCount--;
}

void InputSystem::AddPressedMButton(unsigned char mButton, WPARAM wParam, LPARAM lParam) noexcept {
	if (s_isMouseBPressed[mButton] == 0b1) return;

	s_isMouseBPressed[mButton] = 0b1;
	s_pMButtonsInfos[mButton] = { wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)) };

	s_pPressedMButtons[s_pressedMButtonsCount] = mButton;
	s_pFromMButtonsToIndex[mButton] = s_pressedMButtonsCount;
	s_pressedMButtonsCount++;
}

void InputSystem::RemovePressedMButton(unsigned char mButton) noexcept {
	if (s_isMouseBPressed[mButton] == 0b0) return;

	s_isMouseBPressed[mButton] = 0b0;

	unsigned char tmp = s_pFromMButtonsToIndex[mButton];
	s_pPressedMButtons[tmp] = s_pPressedMButtons[s_pressedMButtonsCount - 1];
	s_pFromMButtonsToIndex[s_pPressedMButtons[tmp]] = tmp;
	s_pressedMButtonsCount--;
}
