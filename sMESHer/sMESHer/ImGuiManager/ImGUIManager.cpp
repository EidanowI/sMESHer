#include "ImGUIManager.h"
#include "../AppWindow/AppWindow.h"
#include "../Renderer/Renderer.h"
#include "../App.h"
#include "../Mesh/Model.h"


extern bool SHOULD_CLOSE_WINDOW_AND_CREATE_NEW;

bool ImGUIManager::S_isCreated = false;
bool ImGUIManager::S_isShowGraphicsSetupWindow = false;
bool ImGUIManager::s_isShowLightEditor = false;
std::string ImGUIManager::s_modeStr = "Show light editor";



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

		char searchBuf[64]{};
		int searchBufSize = 0;
		if (ImGui::InputText("Serch model", searchBuf, sizeof(searchBuf))) {
			for (int i = 0; i < 64; i++) {
				if (searchBuf[i] == 0) {
					searchBufSize = i;
					Scene::BinarySearchModels(searchBuf, searchBufSize);
					
					break;
				}
			}
		}
		

		ImGui::ListBox("##", &Scene::m_selectedModelIndex, Scene::m_namesCstr.data(), Scene::m_namesCstr.size(), 8);
		ImGui::SeparatorText("Edit propertyes");
		if (!Scene::m_models.empty()) {
			if (ImGui::InputText("Name", Scene::m_models[Scene::m_selectedModelIndex]->m_name, 64)) {
				for (int i = 0; i < 64; i++) {
					if (Scene::m_models[Scene::m_selectedModelIndex]->m_name[i] == '\0') {
						
							Scene::m_models[Scene::m_selectedModelIndex]->m_nameLength = i;
							Scene::SortModels();
						
						break;
					}
				}
			}

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
			ImGui::SeparatorText("Color");
			ImGui::ColorPicker4("ModleColor", Scene::m_models[Scene::m_selectedModelIndex]->m_vertConstBuf1Struct.color);
			Scene::m_models[Scene::m_selectedModelIndex]->UpdateConstBuffer();		
		}
		ImGui::End();
	}
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
				Scene::SortModels();
			}
			if (ImGui::MenuItem("Change\nGPU")) {
				S_isShowGraphicsSetupWindow = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::MenuItem((s_modeStr).c_str()))
		{
			if (s_isShowLightEditor) {
				s_isShowLightEditor = false;
				s_modeStr = "Show light editor";
			}
			else {
				s_isShowLightEditor = true;
				s_modeStr = "Hide light editor";
			}
		}

		ImGui::EndMainMenuBar();
	}
}
void ImGUIManager::ShowLightEditor() noexcept {
	if (!s_isShowLightEditor) return;

	ImGui::Begin("Light editor");
	ImGui::SeparatorText("Common");
	ImGui::DragFloat("Pow Factor", &Scene::s_pLight->m_pixlConstBuf1Struct.powFactor,1.0f, 0, 256);
	ImGui::DragFloat("Ambient", &Scene::s_pLight->m_pixlConstBuf1Struct.ambient, 0.01f, 0, 1);

	ImGui::SeparatorText("Point Light");
	ImGui::DragFloat("From center length", &Scene::s_pLight->m_pointLightFromCenterRadius, 0.005f);
	ImGui::DragFloat("Rotation X", &Scene::s_pLight->m_pointLightRotation.x, 0.005f);
	ImGui::DragFloat("Rotation Y", &Scene::s_pLight->m_pointLightRotation.y, 0.005f);
	ImGui::DragFloat("Rotation Z", &Scene::s_pLight->m_pointLightRotation.z, 0.005f);
	ImGui::DragFloat("Specular", &Scene::s_pLight->m_pixlConstBuf1Struct.pointLightSpecular, 0.001f);
	ImGui::DragFloat("Constant", &Scene::s_pLight->m_pixlConstBuf1Struct.pointLightConstant, 0.001f);
	ImGui::DragFloat("Linear", &Scene::s_pLight->m_pixlConstBuf1Struct.pointLightLinear, 0.001f);
	ImGui::DragFloat("Quadratic", &Scene::s_pLight->m_pixlConstBuf1Struct.pointLightQuadratic, 0.001f);
	ImGui::ColorEdit3("Light Color", &Scene::s_pLight->m_pixlConstBuf1Struct.pointLightColor.x);
	if (ImGui::Checkbox("Enable Random Rotation", &Scene::s_pLight->m_isEnableRandomRotation)) {
		if (Scene::s_pLight->m_isEnableRandomRotation) {
			Scene::s_pLight->m_directionalLightRotation = GraphicsFundament::Vector3D(std::lerp(Scene::s_pLight->m_pointLightRotation.x, Scene::s_pLight->m_aspirRotation.x, Scene::s_pLight->m_aspirProgress),
				std::lerp(Scene::s_pLight->m_pointLightRotation.y, Scene::s_pLight->m_aspirRotation.y, Scene::s_pLight->m_aspirProgress),
				std::lerp(Scene::s_pLight->m_pointLightRotation.z, Scene::s_pLight->m_aspirRotation.z, Scene::s_pLight->m_aspirProgress));
			Scene::s_pLight->m_aspirRotation = Scene::s_pLight->m_directionalLightRotation;
			Scene::s_pLight->m_aspirProgress = 1.0f;
		}
	}

	ImGui::SeparatorText("Directional Light");
	ImGui::DragFloat("Direction X", &Scene::s_pLight->m_directionalLightRotation.x, 0.005f);
	ImGui::DragFloat("Direction Y", &Scene::s_pLight->m_directionalLightRotation.y, 0.005f);
	ImGui::DragFloat("Direction Z", &Scene::s_pLight->m_directionalLightRotation.z, 0.005f);
	ImGui::ColorEdit3("Sun Color", &Scene::s_pLight->m_pixlConstBuf1Struct.dirLightColor.x);
	ImGui::DragFloat("Sun Specular", &Scene::s_pLight->m_pixlConstBuf1Struct.dirLightSpecular, 0.001f);

	ImGui::End();
}

void ImGUIManager::EnableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
}
void ImGUIManager::DisableImGuiMouse()
{
	ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
}

char* ImGUIManager::ReadFileName() noexcept {
	OPENFILENAMEA ofn = { 0 };
	char* szFile = new char[260]();
	char* szFileTitle = new char[260]();
	// Initialize remaining fields of OPENFILENAME structure
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = AppWindow::s_hWnd;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = 260 * 2;
	ofn.lpstrFilter = "Model\0*.DAE;*.FBX;*.OBJ\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = szFileTitle;
	ofn.nMaxFileTitle = 260 * 2;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


	char* a = nullptr;
	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		//Display the selected file. 
		return ofn.lpstrFile;

	}

	return a;
}