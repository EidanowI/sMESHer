#include "Mesh.h"

#include "../ShaderSystem/ShaderSystem.h"
#include "../Renderer/Renderer.h"

#include <d3d11.h>

Mesh::Mesh() noexcept
	:
	m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr),
	m_pInputLayout(nullptr),
	m_pSourceVerticies(nullptr),
	m_sourceVerticiesNum(0u),
	m_pSourceIndecies(nullptr),
	m_sourceIndeciesNum(0u)
{}
Mesh::Mesh(Vertex48B* verticies, const unsigned int verticiesNums, TrianglePoly* indecies, const unsigned int indeciesNum, unsigned short vertexShaderIndex) noexcept
	:
	m_pSourceVerticies(verticies), m_sourceVerticiesNum(verticiesNums), m_pSourceIndecies(indecies), m_sourceIndeciesNum(indeciesNum)
{
	int hres = 0;
	D3D11_BUFFER_DESC verticiesBufferDesc{};
	{
		verticiesBufferDesc.ByteWidth = m_sourceVerticiesNum * sizeof(Vertex48B);
		verticiesBufferDesc.Usage = D3D11_USAGE_DEFAULT;// D3D11_USAGE_IMMUTABLE can gives more fps
		verticiesBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;//
		verticiesBufferDesc.CPUAccessFlags = 0u;
		verticiesBufferDesc.MiscFlags = 0u;// miscellaneous
		verticiesBufferDesc.StructureByteStride = sizeof(Vertex48B);
	}
	D3D11_SUBRESOURCE_DATA verticiesSubResData{};
	{
		verticiesSubResData.pSysMem = (void*)m_pSourceVerticies;
	}
	Renderer::s_pDevice->CreateBuffer(&verticiesBufferDesc, &verticiesSubResData, &m_pVertexBuffer);
	

	D3D11_BUFFER_DESC indeciesBufferDesc{};
	{
		indeciesBufferDesc.ByteWidth = m_sourceIndeciesNum * sizeof(TrianglePoly);
		indeciesBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indeciesBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indeciesBufferDesc.CPUAccessFlags = 0u;
		indeciesBufferDesc.MiscFlags = 0u;
		indeciesBufferDesc.StructureByteStride = sizeof(unsigned short);
	}
	D3D11_SUBRESOURCE_DATA indeciesSubData{};
	{
		indeciesSubData.pSysMem = (void*)m_pSourceIndecies;
	}
	Renderer::s_pDevice->CreateBuffer(&indeciesBufferDesc, &indeciesSubData, &m_pIndexBuffer);
	D3D11_INPUT_ELEMENT_DESC inputElementDesc[4] = {
	{"Position", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->position, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Normal", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->normal, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"UV", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->UV, D3D11_INPUT_PER_VERTEX_DATA,  0u},
	{"Color", 0u, DXGI_FORMAT_R32G32B32A32_FLOAT, 0u, (UINT) & ((Vertex48B*)0)->color, D3D11_INPUT_PER_VERTEX_DATA,  0u}
	};

	Renderer::s_pDevice->CreateInputLayout(inputElementDesc,
		4u,
		ShaderSystem::S_VertexShaders[vertexShaderIndex].shaderSourceCode.data(),
		ShaderSystem::S_VertexShaders[vertexShaderIndex].shaderSourceCode.size(),
		&m_pInputLayout);
	//ERROR_HRESULT(Graphics::GetDevice()->CreateInputLayout(Vertex::inputElementDesc,
		//Vertex48B::elementDescNum,
		//tmp->GetVertexShaderByIndex(vertexShaderIndex).shaderSourceCode.data(),
		//tmp->GetVertexShaderByIndex(vertexShaderIndex).shaderSourceCode.size(),
		//&m_pInputLayout));
	//m_pInputLayout = Vertex::CreateInputLayout(m_vertexShaderIndex);
}
Mesh::~Mesh() noexcept {
	if (m_pVertexBuffer != nullptr) {
		m_pVertexBuffer->Release();
		m_pVertexBuffer = nullptr;
	}
	if (m_pIndexBuffer != nullptr) {
		m_pIndexBuffer->Release();
		m_pIndexBuffer = nullptr;
	}
	if (m_pInputLayout != nullptr) {
		m_pInputLayout->Release();
		m_pInputLayout = nullptr;
	}

	if (m_pSourceVerticies != nullptr) {
		delete[] m_pSourceVerticies;
		m_pSourceVerticies = nullptr;
	}
	if (m_pSourceIndecies != nullptr) {
		delete[] m_pSourceIndecies;
		m_pSourceIndecies = nullptr;
	}
}

void Mesh::Bind() noexcept {
	unsigned int strides[] = { sizeof(Vertex48B) };
	unsigned int offsets[] = { 0u };
	Renderer::s_pDeviceContext->IASetVertexBuffers(0u, 1u, &m_pVertexBuffer, strides, offsets);
	Renderer::s_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0u);
	Renderer::s_pDeviceContext->IASetInputLayout(m_pInputLayout);
	Renderer::s_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
void Mesh::Draw() noexcept {
	Bind();
	Renderer::s_pDeviceContext->DrawIndexed(m_sourceIndeciesNum * 3u, 0u, 0u);
}