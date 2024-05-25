#include "Camera.h"
#include "../AppWindow/AppWindow.h"
#include "../Renderer/Renderer.h"

Camera::Camera(const GraphicsFundament::Vector3D& pos, const GraphicsFundament::Vector3D& rotation) noexcept {
	m_position = pos;
	m_camOrientation = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	m_projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, (float)APP_WINDOW_WIDTH / (float)APP_WINDOW_HEIGHT, 0.1f, 100.0f);
	UpdateVectors();

	m_testVert_Buf0.projection = m_projection;
	m_testVert_Buf0.view = m_view;

	m_pTestVert_Buf0 = Renderer::CreateConstBuffer((char*) & m_testVert_Buf0, sizeof(Camera::TestVert_Buf0));
}


void Camera::UpdateVectors() noexcept {
	DirectX::XMVECTOR tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f), m_camOrientation);
	m_right = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), m_camOrientation);
	m_up = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));

	tmp = DirectX::XMVector4Transform(DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f), m_camOrientation);
	m_forward = GraphicsFundament::Vector3D(DirectX::XMVectorGetX(tmp), DirectX::XMVectorGetY(tmp), DirectX::XMVectorGetZ(tmp));
}


GraphicsFundament::Vector3D Camera::GetRight() const noexcept {
	return m_right;
}

GraphicsFundament::Vector3D Camera::GetUp() const noexcept {
	return m_up;
}

GraphicsFundament::Vector3D Camera::GetForward() const noexcept {
	return m_forward;
}


void Camera::SetCameraFPSRotation(float x, float y, float z) noexcept {
	m_camOrientation = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f), z);
	m_camOrientation *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f), x);
	m_camOrientation *= DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f), y);
	UpdateVectors();
	GraphicsFundament::Vector3D lookDir = m_position + m_forward;
	DirectX::XMVECTOR lookTarget = DirectX::XMVectorSet(
		lookDir.x, lookDir.y, lookDir.z,
		0.0f)
		;
	m_view = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(m_position.x, m_position.y, m_position.z, 1.0f), lookTarget, DirectX::XMVectorSet(m_up.x, m_up.y, m_up.z, 1.0f));

	m_testVert_Buf0.projection = m_projection;
	m_testVert_Buf0.view = m_view;
	Renderer::UpdateConstBuffer(m_pTestVert_Buf0, &m_testVert_Buf0, sizeof(Camera::TestVert_Buf0));
}

void Camera::TranslatePosition(float a, GraphicsFundament::Vector3D& direction) noexcept {
	m_position += (direction * a);
}


void Camera::Bind() noexcept {
		Renderer::s_pDeviceContext->VSSetConstantBuffers(0u, 1u, m_pTestVert_Buf0.GetAddressOf());
}

void Camera::UpdateFpsRotation() noexcept {
	m_xRotation -= (float)InputSystem::s_cursorDeltaY / SENSITIVITY_Y;
	m_xRotation = GraphicsFundament::Clamp(DOWN_LOOK_LIMIT, UP_LOOK_LIMIT, m_xRotation);
	m_yRotation += (float)InputSystem::s_cursorDeltaX / SENSITIVITY_X;
	SetCameraFPSRotation(m_xRotation, m_yRotation, m_zRotation);
}