#pragma once


#include "AppWindow/AppWindow.h"
#include "Renderer/Renderer.h"
#include "ImGuiManager/ImGUIManager.h"
#include "GlobalDefs.h"
#include "Mesh/Model.h"
#include "Scene/Scene.h"



class App {
public:
	App() = delete;
	~App() = delete;
	App(const App& o) = delete;

private:
	static int MainLoop() noexcept {
		Renderer::SetCamera();
		while (!m_isShouldCloseWindowAndCreateNew) {
			const std::optional<int> proc = AppWindow::ProcessMessage();
			if (proc.has_value()) return proc.value();

			Renderer::SetRenderTargets();
			Renderer::ClearRenderTarget();
			ImGUIManager::NewFrame();

			ImGUIManager::ShowGUIWindows();

			ImGUIManager::ShowMenuBar();

			Renderer::s_pCamera->UpdateFpsRotation();
			Renderer::s_pCamera->Bind();

			Scene::Render();

			//ImGUIManager::Render();

			ImGUIManager::Render();

			Renderer::PresentRenderTargets();
			InputSystem::UpdateInput();
		}

		return TYLER_DURDEN;
	}

public:
	static int Run() noexcept {
		ImGUIManager imGuiManager = ImGUIManager();
		do {
			InputSystem::Initialize();
			InputSystem::PopulateWithStandarts();
			m_isShouldCloseWindowAndCreateNew = false;
			//if (m_isShouldCloseWindowAndCreateNew) {
			//	m_isShouldCloseWindowAndCreateNew = false;
			//	Renderer::Recreate();
			//}
			//else {
				AppWindow::Initialize();
				Renderer::Initialize();
			//}

			MainLoop();

			InputSystem::Terminate();
			ShaderSystem::Clear();

		} while (m_isShouldCloseWindowAndCreateNew);
		
		Renderer::Terminate();
		AppWindow::Terminate();
		return TYLER_DURDEN;
	}

private:
	static bool m_isShouldCloseWindowAndCreateNew;
};

bool App::m_isShouldCloseWindowAndCreateNew = false;