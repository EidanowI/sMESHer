#pragma once
#include <optional>

#include <Windows.h>



#define APP_WINDOW_WIDTH 1920
#define APP_WINDOW_HEIGHT 1080

class AppWindow {
public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static std::optional<int> ProcessMessage() noexcept;

	static HINSTANCE s_hInstance;
	static HWND s_hWnd;
	static wchar_t s_pRegisteredClassName[8];
};