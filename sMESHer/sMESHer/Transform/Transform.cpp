#include "Transform.h"

void Transform::SetPosition(const GraphicsFundament::Vector3D& position) noexcept {
	m_position = position;
	UpdateTransform();
}
void Transform::SetPosition(float x, float y, float z) noexcept {
	m_position = GraphicsFundament::Vector3D(x, y, z);
	UpdateTransform();
}
GraphicsFundament::Vector3D Transform::GetPosition() const noexcept {
	return m_position;
}
void Transform::SetRotation(const GraphicsFundament::Vector3D& rotation) noexcept {
	m_rotation = rotation;
	UpdateTransform();
}
void Transform::SetRotation(float x, float y, float z) noexcept {
	m_rotation = GraphicsFundament::Vector3D(x, y, z);;
	UpdateTransform();
}
GraphicsFundament::Vector3D Transform::GetRotation() const noexcept {
	return m_rotation;
}
void Transform::SetScale(const GraphicsFundament::Vector3D& scale) noexcept {
	m_scale = scale;
	UpdateTransform();
}
void Transform::SetScale(float x, float y, float z) noexcept {
	m_scale = GraphicsFundament::Vector3D(x, y, z);;
	UpdateTransform();
}
GraphicsFundament::Vector3D Transform::GetScale() const noexcept {
	return m_scale;
}

void Transform::UpdateConstBuffer() noexcept {}
void Transform::UpdateTransform() noexcept {
	m_transform = DirectX::XMMatrixScaling(m_scale.x, m_scale.y, m_scale.z) *
		DirectX::XMMatrixRotationRollPitchYaw(m_rotation.x, m_rotation.y, m_rotation.z) *
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z);
	UpdateConstBuffer();
}