#pragma once

#include <d3d11.h>
#include <string>
#include <vector>
#include <fstream>
#include <wrl.h>

#include "../Dependencies/assimp/include/assimp/Importer.hpp";
#include "../Dependencies/assimp/include/assimp/scene.h";
#include "../Dependencies/assimp/include/assimp/postprocess.h";

#include "../Transform/Transform.h"
#include "../ShaderSystem/ShaderSystem.h"
#include "../Renderer/Renderer.h"
#include "Mesh.h"


class Model : public Transform{
	friend class ImGUIManager;
	friend class Scene;
public:
	Model(const std::string& path) noexcept;
	Model(Mesh* pMesh) noexcept;
	~Model() noexcept;

	void Draw() noexcept;
	void UpdateConstBuffer() noexcept;

	void LoadIntoScene() noexcept;

private:
	Mesh* LoadOne(const aiNode* node, const aiScene* scene);
	void LoadNode(const aiNode* node, const aiScene* scene);
	Mesh* LoadMesh(const aiMesh* mesh, const aiScene* scene);

	Mesh* m_pMeshe;

	ShaderSystem::TestVert_CBuf1 m_vertConstBuf1Struct;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertConstBuf1;

	unsigned short m_vertexShaderIndex;
	unsigned short m_pixelShaderIndex;

public:
	char m_name[64];
	int m_nameLength;
	int m_vertexCount;
};