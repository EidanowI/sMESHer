#pragma once
#include "../Mesh/Model.h"
#include <vector>





class Scene {
public:


	static void Clear() noexcept {
		m_models.clear();
	}
	static void Render() noexcept {
		for (int i = 0; i < m_models.size(); i++) {
			m_models[i]->Draw();
		}
	}
	static void AddModel(const char* path) noexcept;

	static std::vector<Model*> m_models;
	static Model* m_pSelectedModel;
};

