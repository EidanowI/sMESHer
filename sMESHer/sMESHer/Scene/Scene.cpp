#include "Scene.h"

std::vector<Model*> Scene::m_models = std::vector<Model*>();
Model* Scene::m_pSelectedModel = nullptr;

void Scene::AddModel(const char* path) noexcept {
	//std::string a = std::string(path);
	//Model* pM = new Model(path);
	//m_models.push_back(pM);
}