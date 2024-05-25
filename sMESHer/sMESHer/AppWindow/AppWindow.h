#pragma once
#include <optional>

#include <Windows.h>
#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"
#include "../InputSystem/InputSystem.h"



#define APP_WINDOW_WIDTH 1800
#define APP_WINDOW_HEIGHT 900

class AppWindow {
	friend class Renderer;
	friend LRESULT WindowProcess(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	friend class ImGUIManager;

public:
	AppWindow() = delete;
	~AppWindow() = delete;
	AppWindow(const AppWindow& other) = delete;

public:
	static void Initialize() noexcept;
	static void Terminate() noexcept;

	static void EnableCursor() noexcept;
	static void DisableCursor() noexcept;

	static std::optional<int> ProcessMessage() noexcept;

private:
	static HINSTANCE s_hInstance;
	static HWND s_hWnd;
	static wchar_t s_pRegisteredClassName[8];
};