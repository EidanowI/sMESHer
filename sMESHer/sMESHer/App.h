#pragma once


#include "AppWindow/AppWindow.h"
#include "Renderer/Renderer.h"
#include "ImGuiManager/ImGUIManager.h"
#include "GlobalDefs.h"
#include "Mesh/Model.h"
#include "Scene/Scene.h"



class App {
	friend class ImGUIManager;
public:
	App() = delete;
	~App() = delete;
	App(const App& o) = delete;

private:
	static int MainLoop() noexcept {
		Renderer::SetCamera();

		Scene::InitLight();
		while (!m_isShouldCloseWindowAndCreateNew) {
			const std::optional<int> proc = AppWindow::ProcessMessage();
			if (proc.has_value()) return proc.value();

			Renderer::SetRenderTargets();
			Renderer::ClearRenderTarget();
			ImGUIManager::NewFrame();

			ImGUIManager::ShowGUIWindows();

			ImGUIManager::ShowMenuBar();
			ImGUIManager::ShowModelViewer();
			ImGUIManager::ShowLightEditor();

			Renderer::s_pCamera->UpdateFpsRotation();
			Renderer::s_pCamera->Bind();

			Scene::UpdateLight();
			Scene::Render();

			//ImGUIManager::Render();

			ImGUIManager::Render();

			Renderer::PresentRenderTargets();
			InputSystem::UpdateInput();
		}
		Scene::TerminateLight();

		return TYLER_DURDEN;
	}

public:
	static int Run() noexcept {
		do {
			m_isShouldCloseWindowAndCreateNew = false;
			ImGUIManager imGuiManager = ImGUIManager();
			AppWindow::Initialize();
			Renderer::Initialize(m_GPUIndex);

			InputSystem::Initialize();
			InputSystem::PopulateWithStandarts();

			MainLoop();

			Scene::Clear();
			InputSystem::Terminate();
			ShaderSystem::Clear();
			Renderer::Terminate();
			AppWindow::Terminate();

		} while (m_isShouldCloseWindowAndCreateNew);
		
		
		return TYLER_DURDEN;
	}

private:
	static bool m_isShouldCloseWindowAndCreateNew;
	static int m_GPUIndex;
	static bool s_isEditMode;
};