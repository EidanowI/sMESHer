#pragma once
#include <optional>

#include <Windows.h>



#define APP_WINDOW_WIDTH 1920
#define APP_WINDOW_HEIGHT 1080

class AppWindow {
	friend class Renderer;
	friend LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	AppWindow() = delete;
	~AppWindow() = delete;
	AppWindow(const AppWindow& other) = delete;

public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static std::optional<int> ProcessMessage() noexcept;

private:
	static HINSTANCE s_hInstance;
	static HWND s_hWnd;
	static wchar_t s_pRegisteredClassName[8];
};