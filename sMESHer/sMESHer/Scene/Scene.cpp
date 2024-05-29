#include "Scene.h"

std::vector<Model*> Scene::m_models = std::vector<Model*>();
std::vector<const char*> Scene::m_namesCstr = std::vector<const char*>();
int Scene::m_selectedModelIndex = 0;


void Scene::AddModel(const char* path) noexcept {
	//std::string a = std::string(path);
	//Model* pM = new Model(path);
	//m_models.push_back(pM);
}
void Scene::OnChangeGPU() noexcept {

	for (int i = 0; i < m_models.size(); i++) {
		m_models[i]->RecreateAfterChangingGPU();
	}
}

void Scene::RecreateNames() noexcept {
	m_selectedModelIndex = 0;

	m_namesCstr.clear();

	for (int i = 0; i < m_models.size(); i++) {
		m_namesCstr.push_back(m_models[i]->m_name);
	}
}