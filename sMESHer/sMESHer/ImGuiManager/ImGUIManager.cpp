#include "ImGUIManager.h"
#include "../AppWindow/AppWindow.h"
#include "../Renderer/Renderer.h"
#include "../App.h"
#include "../Mesh/Model.h"


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
			if (ImGui::MenuItem("New")) {
				Scene::Clear();
			}
			if (ImGui::MenuItem("Save As")) {}
			if (ImGui::MenuItem("Add")) {
				Model m(nullptr);
				m.LoadIntoScene();

				//Model* mdl = new Model();
				//Scene::m_models.push_back(mdl);
			}
			if (ImGui::MenuItem("Change\nGPU")) {
				S_isShowGraphicsSetupWindow = true;
				//App::m_isShouldCloseWindowAndCreateNew = true;
			}
			//ShowExampleMenuFile();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu(("Mode" + s_modeStr).c_str()))
		{
			if (ImGui::MenuItem("Object Mode")) {
				s_modeStr = " - (OBJECT)";
				App::s_isEditMode = false;
			}
			if (ImGui::MenuItem("Edit Mode")) {
				s_modeStr = " - (EDIT)";
				App::s_isEditMode = true;
			}
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void ImGUIManager::EnableGpuSwitching() noexcept {

}

void ImGUIManager::ShowGUIWindows() noexcept {
	if (S_isShowGraphicsSetupWindow) {
		ImGui::Begin("GPU Selector");
		ImGui::SetWindowSize(ImVec2(400, 80));

		static int gpuIndex = 0;

		std::vector<char*> gpuNames;
		for (int i = 0; i < Renderer::m_availableDevices.size(); i++) {
			gpuNames.push_back(Renderer::m_availableDevices[i].name);
		}
		ImGui::Combo(" ", &gpuIndex, gpuNames.data(), Renderer::m_availableDevices.size());
		if (gpuIndex != App::m_GPUIndex) {
			if (ImGui::Button("Change GPU")) {
				App::m_GPUIndex = gpuIndex;
				App::m_isShouldCloseWindowAndCreateNew = true;
				S_isShowGraphicsSetupWindow = false;
			}
		}
		ImGui::End();

	}

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

void ImGUIManager::ShowModelViewer() noexcept {
	static bool isInit = false;
	if (!App::s_isEditMode) {
		ImGui::Begin("Model Editor");
		if (!isInit) {
			ImGui::SetWindowPos(ImVec2(1510, 140));
			ImGui::SetWindowSize(ImVec2(343, 655));
			isInit = true;
		}
		ImGui::SeparatorText("Models on scene");
		ImGui::ListBox("", &Scene::m_selectedModelIndex, Scene::m_namesCstr.data(), Scene::m_namesCstr.size(), 8);
		ImGui::SeparatorText("Edit propertyes");
		if (!Scene::m_models.empty()) {
			ImGui::InputText("Name", Scene::m_models[Scene::m_selectedModelIndex]->m_name, 64);

			ImGui::SeparatorText("Position");
			float posX = Scene::m_models[Scene::m_selectedModelIndex]->m_position.x;
			float posY = Scene::m_models[Scene::m_selectedModelIndex]->m_position.y;
			float posZ = Scene::m_models[Scene::m_selectedModelIndex]->m_position.z;
			ImGui::DragFloat("PositionX", &posX, 0.005f);
			ImGui::DragFloat("PositionY", &posY, 0.005f);
			ImGui::DragFloat("PositionZ", &posZ, 0.005f);
			if (Scene::m_models[Scene::m_selectedModelIndex]->m_position.x != posX ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_position.y != posY ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_position.z != posZ) 
			{
				Scene::m_models[Scene::m_selectedModelIndex]->SetPosition(posX, posY, posZ);
			}

			ImGui::SeparatorText("Rotation");
			float rotX = Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.x;
			float rotY = Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.y;
			float rotZ = Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.z;
			ImGui::DragFloat("RotationX", &rotX, 0.005f);
			ImGui::DragFloat("RotationY", &rotY, 0.005f);
			ImGui::DragFloat("RotationZ", &rotZ, 0.005f);
			if (Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.x != rotX ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.y != rotY ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_rotation.z != rotZ)
			{
				Scene::m_models[Scene::m_selectedModelIndex]->SetRotation(rotX, rotY, rotZ);
			}

			ImGui::SeparatorText("Scale");
			float scaleX = Scene::m_models[Scene::m_selectedModelIndex]->m_scale.x;
			float scaleY = Scene::m_models[Scene::m_selectedModelIndex]->m_scale.y;
			float scaleZ = Scene::m_models[Scene::m_selectedModelIndex]->m_scale.z;
			ImGui::DragFloat("ScaleX", &scaleX, 0.005f);
			ImGui::DragFloat("ScaleY", &scaleY, 0.005f);
			ImGui::DragFloat("ScaleZ", &scaleZ, 0.005f);
			if (Scene::m_models[Scene::m_selectedModelIndex]->m_scale.x != scaleX ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_scale.y != scaleY ||
				Scene::m_models[Scene::m_selectedModelIndex]->m_scale.z != scaleZ)
			{
				Scene::m_models[Scene::m_selectedModelIndex]->SetScale(scaleX, scaleY, scaleZ);
			}
		}
		ImGui::End();
	}
}