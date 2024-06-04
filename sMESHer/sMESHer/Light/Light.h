#pragma once
#include "../Utils/GparhicsFundamentals.h"
#include <wrl.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3d11.h>
#include "../Mesh/Model.h"


class Light {
public:
	struct PixelBuf0Struct {
		GraphicsFundament::Vector3D viewPosition;
		float powFactor = 32;
		GraphicsFundament::Vector3D viewDirection;
		float ambient = 0.1f;

		GraphicsFundament::Vector3D pointLightPos;
		float pointLightSpecular = 0.3f;
		GraphicsFundament::Vector3D pointLightColor = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
		float tmp;
		float pointLightConstant = 1.0f;
		float pointLightLinear = 0.09f;
		float pointLightQuadratic = 0.032f;
		float tmp2;

		GraphicsFundament::Vector3D dirLightDirection;
		float tmp3;
		GraphicsFundament::Vector3D dirLightColor = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
		float dirLightSpecular = 0.3f;
	};

	float RandomValue();

	Light() noexcept;

	void Update() noexcept;
	void UpdateAsper() noexcept;

	float m_pointLightFromCenterRadius = 6;
	GraphicsFundament::Vector3D m_pointLightRotation;

	GraphicsFundament::Vector3D m_aspirRotation;
	float m_aspirProgress = 0.0f;

	GraphicsFundament::Vector3D m_directionalLightRotation;

	PixelBuf0Struct m_pixlConstBuf1Struct;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertConstBuf1;

	Model m_lamp = Model("Res/Lamp.dae");

	bool m_isEnableRandomRotation = true;
};