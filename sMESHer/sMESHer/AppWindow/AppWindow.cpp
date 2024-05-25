#include "AppWindow.h"
#include <string>
#include "../Renderer/Renderer.h"

HINSTANCE AppWindow::s_hInstance = nullptr;
HWND AppWindow::s_hWnd = nullptr;
wchar_t AppWindow::s_pRegisteredClassName[8] = L"sMESHer";

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

void AppWindow::Initialize() noexcept{
	if (s_hInstance == nullptr) {
		s_hInstance = GetModuleHandle(nullptr);
	}

	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(wc);
	wc.style = CS_CLASSDC;
	wc.lpfnWndProc = WindowProcess;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = s_hInstance;
	wc.hIcon = (HICON)0;
	wc.hCursor = nullptr;
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = nullptr;
	wc.lpszClassName = s_pRegisteredClassName;
	wc.hIconSm = (HICON)0;
	RegisterClassEx(&wc);

	RECT rect;
	rect.left = 100;
	rect.right = rect.left + APP_WINDOW_WIDTH;
	rect.top = 100;
	rect.bottom = rect.top + APP_WINDOW_HEIGHT;

	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;
	AdjustWindowRect(&rect, dwStyle, FALSE);

	s_hWnd = CreateWindow(
		s_pRegisteredClassName, L"sMESHer editor",
		dwStyle,
		(GetSystemMetrics(SM_CXSCREEN) - APP_WINDOW_WIDTH) / 2,///takes centre of a screen
		(GetSystemMetrics(SM_CYSCREEN) - APP_WINDOW_HEIGHT) /2 ,///takes centre of a screen
		APP_WINDOW_WIDTH,
		APP_WINDOW_HEIGHT,
		nullptr, nullptr, s_hInstance, 0
	);

	ShowWindow(s_hWnd, SW_SHOWDEFAULT);
	UpdateWindow(s_hWnd);

	ImGui::StyleColorsLight();

	ImGui_ImplWin32_Init(s_hWnd);

	RAWINPUTDEVICE rawInDevice;
	rawInDevice.usUsagePage = 0x01;/// for generic desktop controls
	rawInDevice.usUsage = 0x02;/// for mouse
	rawInDevice.dwFlags = 0u;
	rawInDevice.hwndTarget = NULL;/// A handle to the target window. If NULL it follows the keyboard focus

	RegisterRawInputDevices(&rawInDevice, 1u, sizeof(RAWINPUTDEVICE));
}
void AppWindow::Terminate() noexcept {
	ImGui_ImplWin32_Shutdown();

	DestroyWindow(s_hWnd);
	UnregisterClass(s_pRegisteredClassName, s_hInstance);
}

void AppWindow::EnableCursor() noexcept {
	while (ShowCursor(TRUE) < 0);
}
void AppWindow::DisableCursor() noexcept {
	while (ShowCursor(FALSE) >= 0);
}

std::optional<int> AppWindow::ProcessMessage() noexcept {
	MSG msg;

	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) return std::optional<int>(msg.wParam);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return std::optional<int>{};
}


LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}

	case WM_INPUT:
	{
		if (InputSystem::s_isCursorReceivesDelta) break;

		UINT size;

		if (GetRawInputData(
			(HRAWINPUT)lParam,
			RID_INPUT,
			nullptr,
			&size,
			sizeof(RAWINPUTHEADER)) == -1)
		{
			break;
		}
		InputSystem::s_rawBuffer.resize(size);

		if (GetRawInputData(
			(HRAWINPUT)lParam,
			RID_INPUT,
			InputSystem::s_rawBuffer.data(),
			&size,
			sizeof(RAWINPUTHEADER)) != size)
		{
			break;
		}

		RAWINPUT ri = *(RAWINPUT*)InputSystem::s_rawBuffer.data();
		if (ri.header.dwType == RIM_TYPEMOUSE &&
			(ri.data.mouse.lLastX != 0 || ri.data.mouse.lLastY != 0))
		{
			if (InputSystem::s_isInputDelta) {
				InputSystem::s_cursorDeltaX = ri.data.mouse.lLastX;
				InputSystem::s_cursorDeltaY = ri.data.mouse.lLastY;
			}
		}
		break;
	}

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		if (!(lParam & 0b01000000000000000000000000000000)) {//30 bit is 1 if key was pressed before this message
			InputSystem::s_ppFunctions_KEY_ONCE[(unsigned char)wParam](lParam);
			InputSystem::AddPressedKey((unsigned char)wParam, lParam);
		}
		break;
	}

	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		InputSystem::s_ppFunctions_KEY_RELEASE[(unsigned char)wParam](lParam);
		InputSystem::RemovePressedKey((unsigned char)wParam);
		break;
	}

	case WM_LBUTTONDOWN:
	{
		//if (wParam == (long long)0b00000101) MessageBox(nullptr, L"LINE: ", L"afaf", MB_OK | MB_ICONEXCLAMATION);
		InputSystem::s_ppFunctions_MOUSE_BUTTON_ONCE[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::LEFT_MOUSE_BUTON, wParam, lParam);
		break;
	}
	case WM_LBUTTONUP:
	{
		InputSystem::s_ppFunctions_MOUSE_BUTTON_RELEASE[InputSystem::LEFT_MOUSE_BUTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::LEFT_MOUSE_BUTON);
		break;
	}

	case WM_RBUTTONDOWN:
	{
		InputSystem::s_ppFunctions_MOUSE_BUTTON_ONCE[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::RIGHT_MOUSE_BUTTON, wParam, lParam);
		break;
	}
	case WM_RBUTTONUP:
	{
		InputSystem::s_ppFunctions_MOUSE_BUTTON_RELEASE[InputSystem::RIGHT_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::RIGHT_MOUSE_BUTTON);
		break;
	}

	case WM_MBUTTONDOWN:
	{
		InputSystem::s_ppFunctions_MOUSE_BUTTON_ONCE[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::AddPressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON, wParam, lParam);
		break;
	}
	case WM_MBUTTONUP:
	{
		InputSystem::s_ppFunctions_MOUSE_BUTTON_RELEASE[InputSystem::MIDDLE_MOUSE_BUTTON](wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::RemovePressedMButton(InputSystem::MIDDLE_MOUSE_BUTTON);
		break;
	}

	case WM_MOUSEWHEEL:
	{
		//if (*(short*)&wParam != (short)0b00000100) break; -- BREAK IF YOU DOESNT HOLD SHIFT WHILE SPIN THE WHEEL
		//InputSystem::func_WHEELMOVE(wParam, *((short*)&lParam), *((short*)((char*)&lParam + 2)));
		InputSystem::s_wheelDelta += (short)*((char*)&wParam + 2);
		while (InputSystem::s_wheelDelta >= WHEEL_DELTA) {
			InputSystem::s_wheelDelta -= WHEEL_DELTA;
			InputSystem::s_pFunction_MOUSE_WHEEL_STEP_UP(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		while (InputSystem::s_wheelDelta <= -WHEEL_DELTA) {
			InputSystem::s_wheelDelta += WHEEL_DELTA;
			InputSystem::s_pFunction_MOUSE_WHEEL_STEP_DOWN(*((short*)(void*)&lParam), *((short*)((char*)&lParam + 2)));
		}
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}