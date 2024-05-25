#pragma once
#include "InputSystem.h"
#include "../AppWindow/AppWindow.h"
#include "../ImGuiManager/ImGUIManager.h"

class InputActionFunctions
{
public:
	static void EMPTYFUNC() noexcept {};
	static void EMPTYFUNC(LPARAM lParam) noexcept {};
	static void EMPTYFUNC(WPARAM wParam, LPARAM lParam) noexcept {};
	static void EMPTYFUNC(WPARAM wParam, int x, int y) noexcept {};
	static void EMPTYFUNC(int x, int y) noexcept {};


	static void PressRightButton(WPARAM wParam, int x, int y) noexcept {
		tagPOINT mousePos;
		GetCursorPos(&mousePos);
		InputSystem::s_currentCursorPosX = mousePos.x;
		InputSystem::s_currentCursorPosY = mousePos.y;
		InputSystem::s_isCursorReceivesDelta = false;
		AppWindow::DisableCursor();
		ImGUIManager::DisableImGuiMouse();
		InputSystem::s_isInputDelta = true;

	}
	static void ReleaseRightButton(WPARAM wParam, int x, int y) noexcept {
		InputSystem::s_isCursorReceivesDelta = true;
		AppWindow::EnableCursor();
		ImGUIManager::EnableImGuiMouse();
		InputSystem::s_isInputDelta = false;
		SetCursorPos(InputSystem::s_currentCursorPosX, InputSystem::s_currentCursorPosY);
	}

	static void Move_Right(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(0.09f, Renderer::s_pCamera->m_right);
	};
	static void Move_Left(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(-0.09f, Renderer::s_pCamera->m_right);
	};
	static void Move_Forward(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(0.09f, Renderer::s_pCamera->m_forward);
	};
	static void Move_Back(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(-0.09f, Renderer::s_pCamera->m_forward);
	};
	static void Move_Forward_By_Wheel(int x, int y) noexcept {
		Renderer::s_pCamera->TranslatePosition(1.2f, Renderer::s_pCamera->m_forward);
	};
	static void Move_Back_By_Wheel(int x, int y) noexcept {
		Renderer::s_pCamera->TranslatePosition(-1.2f, Renderer::s_pCamera->m_forward);
	};
	static void Move_Up(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(0.09f, Renderer::s_pCamera->m_up);
	};
	static void Move_Down(LPARAM lParam) noexcept {
		if (InputSystem::s_isInputDelta)
			Renderer::s_pCamera->TranslatePosition(-0.09f, Renderer::s_pCamera->m_up);
	};
};