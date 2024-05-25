#include "Model.h"
#include "../ImGuiManager/ImGUIManager.h"


/*Model::Model(const char* path) noexcept {
	m_vertexShaderIndex = ShaderSystem::GetVertexShaderIndex("VertexSh", "Shaders/");
	m_pixelShaderIndex = ShaderSystem::GetPixelShaderIndex("PixelSha", "Shaders/");

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_FlipUVs | aiProcess_Triangulate);
	if (scene == nullptr ||
		scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {

		return;
	}
	m_meshes = std::vector<char*>();
	LoadNode(scene->mRootNode, scene);

	m_position = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_rotation = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));
	m_pVertConstBuf1 =Renderer::CreateConstBuffer((char*)&m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));

	UpdateTransform();
}*/

Model::Model(Mesh* pMesh) noexcept{
	m_vertexShaderIndex = ShaderSystem::GetVertexShaderIndex("VertexSh", "Shaders/");
	m_pixelShaderIndex = ShaderSystem::GetPixelShaderIndex("PixelSha", "Shaders/");

	m_pMeshe = pMesh;

	m_position = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_rotation = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));
	m_pVertConstBuf1 = Renderer::CreateConstBuffer((char*)&m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));

	UpdateTransform();
}
/*Model::Model() noexcept {
	m_vertexShaderIndex = ShaderSystem::GetVertexShaderIndex("VertexSh", "Shaders/");
	m_pixelShaderIndex = ShaderSystem::GetPixelShaderIndex("PixelSha", "Shaders/");

	char* a = ImGUIManager::ReadFileName();

	Assimp::Importer importer;
	const aiScene* sceneee = importer.ReadFile(a, aiProcess_FlipUVs | aiProcess_Triangulate);
	if (sceneee == nullptr ||
		sceneee->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!sceneee->mRootNode) {

		return;
	}
	m_meshes = std::vector<char*>();
	LoadNode(sceneee->mRootNode, sceneee);

	m_position = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_rotation = GraphicsFundament::Vector3D(0.0f, 0.0f, 0.0f);
	m_scale = GraphicsFundament::Vector3D(1.0f, 1.0f, 1.0f);
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));
	m_pVertConstBuf1 = Renderer::CreateConstBuffer((char*)&m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));

	UpdateTransform();
}*/
Model::~Model() noexcept {
	/*for (int i = 0; i < m_meshes.size(); i++) {
		delete ((Mesh*)m_meshes[i]);
	}*/

	delete m_pMeshe;
}

void Model::LoadNode(const aiNode* node, const aiScene* scene) {
	// process all the node's meshes (if any)
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Model* pModel = new Model(LoadMesh(mesh, scene));
		pModel->m_position = GraphicsFundament::Vector3D(node->mTransformation.a4, node->mTransformation.b4, node->mTransformation.c4);
		pModel->UpdateTransform();
		Scene::m_models.push_back(pModel);
		//m_meshes.push_back((char*)LoadMesh(mesh, scene));
	}

	for (unsigned int i = 0; i< node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}
Mesh* Model::LoadMesh(const aiMesh* mesh, const aiScene* scene) {
	unsigned int verticiesNums = mesh->mNumVertices;
	Mesh::Vertex48B* verticies = new Mesh::Vertex48B[verticiesNums];
	unsigned int indeciesNum = mesh->mNumFaces;
	Mesh::TrianglePoly* indecies = new Mesh::TrianglePoly[indeciesNum];

	bool isMeshContainsUV = mesh->mTextureCoords[0];
	int theBigOne = 0;
	
	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		verticies[i].position = GraphicsFundament::Vector3D(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		verticies[i].normal = GraphicsFundament::Vector3D(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		if (isMeshContainsUV)
		{
			verticies[i].UV = GraphicsFundament::Vector2D(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
		}
		else 
		{
			verticies[i].UV = GraphicsFundament::Vector2D(0.0f, 0.0f);
		}
		if (mesh->mColors[3]) {
			verticies[i].color = GraphicsFundament::Vector4D(mesh->mColors[i]->r, mesh->mColors[i]->g, mesh->mColors[i]->b, mesh->mColors[i]->a);
		}
		else {
			verticies[i].color = GraphicsFundament::Vector4D(0.0f,0.0f,0.0f,0.0f);
		}
	}
	for (int i = 0; i < indeciesNum; i++) {
		indecies[i].I1 = (unsigned short)mesh->mFaces[i].mIndices[0];
		indecies[i].I2 = (unsigned short)mesh->mFaces[i].mIndices[1];
		indecies[i].I3 = (unsigned short)mesh->mFaces[i].mIndices[2];
	}

	return new Mesh(verticies, verticiesNums,
		indecies,indeciesNum,
		m_vertexShaderIndex);
}


void Model::Draw() noexcept {
	Renderer::s_pDeviceContext->VSSetConstantBuffers(1u, 1u, m_pVertConstBuf1.GetAddressOf());
	Renderer::s_pDeviceContext->VSSetShader(ShaderSystem::S_VertexShaders[m_vertexShaderIndex].vertexShader.Get(), nullptr, 0u);
	Renderer::s_pDeviceContext->PSSetShader(ShaderSystem::S_PixelShaders[m_pixelShaderIndex].pixelShader.Get(), nullptr, 0u);


	m_pMeshe->Bind();
	m_pMeshe->Draw();
	/*for (int i = 0; i < m_meshes.size(); i++) {
		((Mesh*)m_meshes[i])->Bind();
		((Mesh*)m_meshes[i])->Draw();
	}*/
}

void Model::UpdateConstBuffer() noexcept {
	m_vertConstBuf1Struct.modelMatrix = m_transform;
	m_vertConstBuf1Struct.normalMatrix = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(nullptr, m_transform));

	Renderer::UpdateConstBuffer(m_pVertConstBuf1, &m_vertConstBuf1Struct, sizeof(ShaderSystem::TestVert_CBuf1));
}

void Model::LoadIntoScene() noexcept {
	char* a = ImGUIManager::ReadFileName();

	Assimp::Importer importer;
	const aiScene* sceneee = importer.ReadFile(a, aiProcess_FlipUVs | aiProcess_Triangulate);
	if (sceneee == nullptr ||
		sceneee->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!sceneee->mRootNode) {

		return;
	}

	Model m(nullptr);
	m.LoadNode(sceneee->mRootNode, sceneee);
}