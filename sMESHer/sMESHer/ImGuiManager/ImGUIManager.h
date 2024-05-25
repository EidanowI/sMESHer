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

	static void EnableImGuiMouse();
	static void DisableImGuiMouse();


	static void ShowMenuBar() noexcept;
	static char* ReadFileName() noexcept {
		OPENFILENAMEA ofn = { 0 };
		char* szFile = new char[260]();
		char* szFileTitle = new char[260]();
		// Initialize remaining fields of OPENFILENAME structure
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = AppWindow::s_hWnd;
		ofn.lpstrFile = szFile;
		ofn.nMaxFile = 260 * 2;
		ofn.lpstrFilter = "Model\0*.DAE\0*.FBX\0*.OBJ\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = szFileTitle;
		ofn.nMaxFileTitle = 260 * 2;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


		char* a = nullptr;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			//Display the selected file. 
			return ofn.lpstrFile;

		}

		return a;
	}

private:
    static std::string s_modeStr;
	static bool S_isCreated;
	static bool S_isShowGraphicsSetupWindow;
};
