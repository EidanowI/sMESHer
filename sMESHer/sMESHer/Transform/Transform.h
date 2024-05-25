#pragma once

#include "../Utils/GparhicsFundamentals.h"


class Transform {
public:
	void SetPosition(const GraphicsFundament::Vector3D& position) noexcept;
	void SetPosition(float x, float y, float z) noexcept;
	GraphicsFundament::Vector3D GetPosition() const noexcept;
	void SetRotation(const GraphicsFundament::Vector3D& rotation) noexcept;
	void SetRotation(float x, float y, float z) noexcept;
	GraphicsFundament::Vector3D GetRotation() const noexcept;
	void SetScale(const GraphicsFundament::Vector3D& scale) noexcept;
	void SetScale(float x, float y, float z) noexcept;
	GraphicsFundament::Vector3D GetScale() const noexcept;

protected:
	virtual void UpdateConstBuffer() noexcept;
	void UpdateTransform() noexcept;

protected:
	DirectX::XMMATRIX m_transform = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);

	GraphicsFundament::Vector3D m_position = GraphicsFundament::Vector3D();
	GraphicsFundament::Vector3D m_rotation = GraphicsFundament::Vector3D();
	GraphicsFundament::Vector3D m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
};
