#pragma once

#include "../Dependencies/imGUI/imgui.h"
#include "../Dependencies/imGUI/imgui_impl_win32.h"
#include "../Dependencies/imGUI/imgui_impl_dx11.h"
#include "../Scene/Scene.h"
#include "../Mesh/Model.h"
#include <string>
#include "fstream"


char* wcharTochar(const wchar_t* _wchar);


/// <summary>
/// GUI for convenient developing and debugging
/// </summary>
class ImGUIManager {
public:
	ImGUIManager() noexcept;
	~ImGUIManager() noexcept;

	static void NewFrame() noexcept;
	static void Render() noexcept;

	static void ShowGUIWindows() noexcept;
	static void ShowModelViewer() noexcept;
	static void ShowMenuBar() noexcept;
	static void ShowLightEditor() noexcept;

	static void EnableImGuiMouse();
	static void DisableImGuiMouse();

	static char* ReadFileName() noexcept;

private:
    static std::string s_modeStr;
	static bool S_isCreated;
	static bool S_isShowGraphicsSetupWindow;
	static bool s_isShowLightEditor;
};
