#pragma once

#include <wrl.h>
#include <vector>
#include "../Utils/GparhicsFundamentals.h"
#include <fstream>
#include <d3d11.h>

/// <summary>
/// Each shader(file with compiled HLSL code) is named in the format Name.cso, where name is an array of 8 elements(For example: TestVets.cso, HaHaVERT.cso and etc. 
/// This array is cast to a unique ID, which is a big number (long long), used to determine whether the shader has been loaded into the ïêôçðøñû card or needs to be loaded from disk.
/// </summary>
class ShaderSystem {
public:
	struct VertexShader {
		long long shader_UID;
		std::vector<char> shaderSourceCode;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
	};
	struct PixelShader {
		long long shader_UID;
		std::vector<char> shaderSourceCode;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
	};
	struct TestVert_CBuf1 {
		DirectX::XMMATRIX modelMatrix;
		DirectX::XMMATRIX normalMatrix;
		float color[4];
	};


	struct PointLight {
		GraphicsFundament::Vector3D m_position;
		float m_specularStrength;
		GraphicsFundament::Vector4D m_color;
		float m_constant;
		float m_linear;
		float m_quadratic;
		float m_ambientStrength;
	};

	struct PixelCBuf0 {
		GraphicsFundament::Vector3D viewPosition;
		float powFactor;
		GraphicsFundament::Vector3D viewDirection;
		float cutofCosine;
		PointLight pointLights;
		unsigned int pointLightNum = 0u;
		float tmp1;
		float tmp2;
		float tmp3;
	};
	/// <summary>
	/// Load shader and gives his index or, if shader already loaded just gives his index
	/// </summary>
	/// <param name="shader_UID"> For example -- "TestVert" </param>
	/// <param name="path"> For example -- "Shaders/" </param>
	/// <returns></returns>
	static unsigned int GetVertexShaderIndex(const char* shader_UID, const std::string& path) noexcept;
	/// <summary>
	/// Load shader and gives his index or, if shader already loaded just gives his index
	/// </summary>
	/// <param name="shader_UID"> For example -- "TestPixl" </param>
	/// <param name="path"> For example -- "Shaders/" </param>
	/// <returns></returns>
	static unsigned int GetPixelShaderIndex(const char* shader_UID, const std::string& path) noexcept;
	static void Clear() noexcept;

	static std::vector<VertexShader> S_VertexShaders;
	static std::vector<PixelShader> S_PixelShaders;
};