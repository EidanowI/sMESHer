#pragma once


#include "AppWindow/AppWindow.h"
#include "Renderer/Renderer.h"
#include "GlobalDefs.h"



class App {
public:
	App() = delete;
	~App() = delete;
	App(const App& o) = delete;

private:
	static int MainLoop() noexcept {
		while (!m_isShouldCloseWindowAndCreateNew) {
			const std::optional<int> proc = AppWindow::ProcessMessage();
			if (proc.has_value()) return proc.value();

			Renderer::Render();
			Renderer::Update();
		}

		return TYLER_DURDEN;
	}

public:
	static int Run() noexcept {
		do {
			m_isShouldCloseWindowAndCreateNew = false;

			AppWindow::Initialize();
			Renderer::Initialize();

			MainLoop();

			AppWindow::Terminate();
			Renderer::Terminate();
		} while (m_isShouldCloseWindowAndCreateNew);
		
		return TYLER_DURDEN;
	}

private:
	static bool m_isShouldCloseWindowAndCreateNew;
};

bool App::m_isShouldCloseWindowAndCreateNew = false;