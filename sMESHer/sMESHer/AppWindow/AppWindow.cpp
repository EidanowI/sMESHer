#include "AppWindow.h"



HINSTANCE AppWindow::s_hInstance = nullptr;
HWND AppWindow::s_hWnd = nullptr;
wchar_t AppWindow::s_pRegisteredClassName[8] = L"sMESHer";

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

	//should to use for correct window size
	RECT rect;
	rect.left = 100;
	rect.right = rect.left + APP_WINDOW_WIDTH;
	rect.top = 100;
	rect.bottom = rect.top + APP_WINDOW_HEIGHT;

	AdjustWindowRect(&rect, WS_MINIMIZEBOX | WS_POPUP, FALSE);
	DWORD dwStyle = WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU;

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

	//ImGui_ImplWin32_Init(S_hWnd);
}
void AppWindow::Terminate() noexcept {
	//ImGui_ImplWin32_Shutdown();
	DestroyWindow(s_hWnd);
	UnregisterClass(s_pRegisteredClassName, s_hInstance);
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
	//if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
	//	return true;

	switch (msg)
	{

	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}