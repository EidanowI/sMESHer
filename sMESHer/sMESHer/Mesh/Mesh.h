#pragma once

#include <d3d11.h>

#include "../Utils/GparhicsFundamentals.h"

class Mesh {
	friend class Model;
public:
	struct Vertex48B {
		GraphicsFundament::Vector3D position;
		GraphicsFundament::Vector3D normal;
		GraphicsFundament::Vector2D UV;
		GraphicsFundament::Vector4D color;
	};
	struct TrianglePoly {
		unsigned short I1;
		unsigned short I2;
		unsigned short I3;
	};

	Mesh(Vertex48B* verticies, const unsigned int verticiesNums, TrianglePoly* indecies, const unsigned int indeciesNum, unsigned short vertexShaderIndex) noexcept;
	Mesh() noexcept;
	~Mesh() noexcept;

	void Bind() noexcept;
	void Draw() noexcept;

public:

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11InputLayout* m_pInputLayout;

	Vertex48B* m_pSourceVerticies;
	unsigned int m_sourceVerticiesNum;
	TrianglePoly* m_pSourceIndecies;
	unsigned int m_sourceIndeciesNum;
};
