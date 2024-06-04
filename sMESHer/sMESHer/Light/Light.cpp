#include "Light.h"
#include "../Renderer/Renderer.h"
#include "../Scene/Scene.h"

Light::Light() noexcept {
	m_pixlConstBuf1Struct.viewPosition = Renderer::s_pCamera->GetPosition();
	m_pixlConstBuf1Struct.viewDirection = Renderer::s_pCamera->GetForward();

	m_pVertConstBuf1 = Renderer::CreateConstBuffer((char*)&m_pixlConstBuf1Struct, sizeof(PixelBuf0Struct));
}

void Light::Update() noexcept {
	m_pixlConstBuf1Struct.viewPosition = Renderer::s_pCamera->GetPosition();
	m_pixlConstBuf1Struct.viewDirection = Renderer::s_pCamera->GetForward();


	DirectX::XMMATRIX pointPosMatrix =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0) *
		DirectX::XMMatrixTranslation(0.0f, m_pointLightFromCenterRadius, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(std::lerp(m_pointLightRotation.x, m_aspirRotation.x, m_aspirProgress),
			std::lerp(m_pointLightRotation.y, m_aspirRotation.y, m_aspirProgress),
			std::lerp(m_pointLightRotation.z, m_aspirRotation.z, m_aspirProgress));

	DirectX::XMVECTOR ver = { 0.0f,0.0f, 0.0f, 1.0f };
	ver = DirectX::XMVector3Transform(ver, pointPosMatrix);

	m_pixlConstBuf1Struct.pointLightPos = GraphicsFundament::Vector3D(
		DirectX::XMVectorGetX(ver),
		DirectX::XMVectorGetY(ver),
		DirectX::XMVectorGetZ(ver)
	);
	m_lamp.SetPosition(m_pixlConstBuf1Struct.pointLightPos);
	m_lamp.SetScale(0.09f, 0.09, 0.09);

	DirectX::XMMATRIX dirPosMatrix =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0) *
		DirectX::XMMatrixTranslation(0.0f, 1, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYaw(m_directionalLightRotation.x, m_directionalLightRotation.y, m_directionalLightRotation.z);

	ver = DirectX::XMVector3Transform(ver, dirPosMatrix);

	m_pixlConstBuf1Struct.dirLightDirection = GraphicsFundament::Vector3D(
		DirectX::XMVectorGetX(ver),
		DirectX::XMVectorGetY(ver),
		DirectX::XMVectorGetZ(ver)
	).Normalized();

	Renderer::UpdateConstBuffer(m_pVertConstBuf1, &m_pixlConstBuf1Struct, sizeof(PixelBuf0Struct));

	Renderer::s_pDeviceContext->PSSetConstantBuffers(0u, 1u, m_pVertConstBuf1.GetAddressOf());

	m_lamp.Draw();
	UpdateAsper();
}

void Light::UpdateAsper() noexcept {
	if (!m_isEnableRandomRotation) return;
	if (m_aspirProgress >= 1.0f) {
		m_aspirProgress = 0;
		m_pointLightRotation = m_aspirRotation;
		m_aspirRotation.x += RandomValue();
		if (m_aspirRotation.x > (3.141592f * 2)) {
			m_aspirRotation.x -= (3.141592f * 2);
		}
		m_aspirRotation.y += RandomValue();
		if (m_aspirRotation.y > (3.141592f * 2)) {
			m_aspirRotation.y-= (3.141592f * 2);
		}
		m_aspirRotation.z += RandomValue();
		if (m_aspirRotation.z > (3.141592f * 2)) {
			m_aspirRotation.z -= (3.141592f * 2);
		}
		
	}
	m_aspirProgress += 0.005;
}
float Light::RandomValue() {
	const long randMax = 100000;
	static long long state = std::time(0);
	state = 214013 * state + 2531011;
	state ^= state >> 15;
	return (state % randMax) / (float)randMax;
}