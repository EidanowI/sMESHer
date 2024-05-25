#pragma once

#include <wrl.h>
#include <d3d11.h>

#include "../Transform/Transform.h"
#include "../InputSystem/InputSystem.h"

#define SENSITIVITY_X 120.0f
#define SENSITIVITY_Y 120.0f
#define UP_LOOK_LIMIT 1.57f
#define DOWN_LOOK_LIMIT -1.57f


class Camera : public Transform{
	struct TestVert_Buf0 {
		DirectX::XMMATRIX view;
		DirectX::XMMATRIX projection;
	};

private:
	void UpdateVectors() noexcept;

public:
	Camera(const GraphicsFundament::Vector3D& pos, const GraphicsFundament::Vector3D& rotation) noexcept;

	void UpdateFpsRotation() noexcept;

	void SetCameraFPSRotation(float x, float y, float z) noexcept;
	void TranslatePosition(float a, GraphicsFundament::Vector3D& direction) noexcept;

	void Bind() noexcept;

	GraphicsFundament::Vector3D GetRight() const noexcept;
	GraphicsFundament::Vector3D GetUp() const noexcept;
	GraphicsFundament::Vector3D GetForward() const noexcept;
public:	

	float m_xRotation = -0.374f;
	float m_yRotation = 2.726f;
	float m_zRotation = 0.0f;

	DirectX::XMMATRIX m_camOrientation;
	GraphicsFundament::Vector3D m_right;
	GraphicsFundament::Vector3D m_up;
	GraphicsFundament::Vector3D m_forward;
	DirectX::XMMATRIX m_view;
	DirectX::XMMATRIX m_projection;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pTestVert_Buf0;///for testVert shader
	TestVert_Buf0 m_testVert_Buf0;
};