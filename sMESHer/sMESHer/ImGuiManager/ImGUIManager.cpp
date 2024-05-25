#include "ImGUIManager.h"
#include "../AppWindow/AppWindow.h"
#include "../Renderer/Renderer.h"


extern bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW;

bool ImGUIManager::S_isCreated = false;
bool ImGUIManager::S_isShowGraphicsSetupWindow = false;
std::string ImGUIManager::s_modeStr = " - (OBJECT)";


char* wcharTochar(const wchar_t* _wchar)
{
	char* _char;
	int len = WideCharToMultiByte(CP_ACP, 0, _wchar, (int)wcslen(_wchar), NULL, 0, NULL, NULL);
	_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, _wchar, (int)wcslen(_wchar), _char, len, NULL, NULL);
	_char[len] = '\0';
	return _char;
}

ImGUIManager::ImGUIManager() noexcept {
	if (S_isCreated) return;
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	ImGui::StyleColorsDark();
	S_isCreated = true;
}

ImGUIManager::~ImGUIManager() noexcept {
	ImGui::DestroyContext();
	S_isCreated = false;
}


void ImGUIManager::NewFrame() noexcept {
	ImGui_ImplWin32_NewFrame();
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void ImGUIManager::Render() noexcept {
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGui::UpdatePlatformWindows();
	ImGui::RenderPlatformWindowsDefault();
}
void ImGUIManager::ShowMenuBar() noexcept {
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("New", "Ctrl + N")) {
				Scene::Clear();
			}
			if (ImGui::MenuItem("Save As", "Ctrl + S")) {}
			if (ImGui::MenuItem("Add", "Ctrl + A")) {
				Model m(nullptr);
				m.LoadIntoScene();

				//Model* mdl = new Model();
				//Scene::m_models.push_back(mdl);
			}
			if (ImGui::MenuItem("Change\nGPU", "Ctrl + G")) {

			}
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(("Mode" + s_modeStr).c_str()))
		{
			if (ImGui::MenuItem("Object Mode", "Tab")) {
				s_modeStr = " - (OBJECT)";
			}
			if (ImGui::MenuItem("Edit Mode", "Tab")) {
				s_modeStr = " - (EDIT)";
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}


void ImGUIManager::ShowGUIWindows() noexcept {
	/*ImGui::Begin("Main Setup Window");
	ImGui::Checkbox("Show Graphics Sepup", &S_isShowGraphicsSetupWindow);
	ImGui::End();

	if (S_isShowGraphicsSetupWindow) {
		ImGui::Begin("Graphics Setup Window");
		ImGui::SeparatorText("Select adapter params");


		const char* cringw[10];
		std::string aa[10];
		for (int i = 0; i < Renderer::s_graphicDevices_SIZE; i++) {
			std::string s(Graphics::S_pGraphicDevices[i].adapterName.begin(), Graphics::S_pGraphicDevices[i].adapterName.end());
			aa[i] = std::to_string(Graphics::S_pGraphicDevices[i].deviceID) + ". "
				+ s + " (score: " + std::to_string(Graphics::S_pGraphicDevices[i].score) + ")";
			cringw[i] = aa[i].c_str();
		}
		static int currentAdapter = -1;
		if (currentAdapter == -1) {
			for (int i = 0; i < Graphics::S_graphicDevices_SIZE; i++) {
				if (Graphics::S_pGraphicDevices[i].deviceID == Window::S_CreateWindowParams.deviceID) {
					currentAdapter = i;
					break;
				}
			}
		}

		ImGui::Combo("Adapter", &currentAdapter, cringw, Graphics::S_graphicDevices_SIZE);
		if (Graphics::S_pGraphicDevices[currentAdapter].deviceID != Window::S_CreateWindowParams.deviceID) {
			ImGui::SameLine();
			if (ImGui::Button("Change Over")) {
				Window::S_CreateWindowParams.deviceID = Graphics::S_pGraphicDevices[currentAdapter].deviceID;
				SHOULD_CLOSE_WINDOW_AND_CREATE_NEW = true;
			}
		}
		ImGui::End();
	}*/
}


void ImGUIManager::EnableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}

void ImGUIManager::DisableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}