#include "Scene.h"

std::vector<Model*> Scene::m_models = std::vector<Model*>();
std::vector<const char*> Scene::m_namesCstr = std::vector<const char*>();
int Scene::m_selectedModelIndex = 0;


void Scene::qsortRecursive(Model** arr, int size) {
    //Указатели в начало и в конец массива
    int i = 0;
    int j = size - 1;

    //Центральный элемент массива
    Model* mid = arr[size / 2];

    //Делим массив
    do {
        //Пробегаем элементы, ищем те, которые нужно перекинуть в другую часть
        //В левой части массива пропускаем(оставляем на месте) элементы, которые меньше центрального
        while (arr[i]->m_nameLength < mid->m_nameLength) {
            i++;
        }
        //В правой части пропускаем элементы, которые больше центрального
        while (arr[j]->m_nameLength > mid->m_nameLength) {
            j--;
        }

        //Меняем элементы местами
        if (i <= j) {
            Model* tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;

            i++;
            j--;
        }
    } while (i <= j);


    //Рекурсивные вызовы, если осталось, что сортировать
    if (j > 0) {
        //"Левый кусок"
        qsortRecursive(arr, j + 1);
    }
    if (i < size) {
        //"Првый кусок"
        qsortRecursive(&arr[i], size - i);
    }
}

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

void Scene::SortModels() noexcept {
	//гарантирует что индекс выбранной в массиве модели будет указывать
	//на ту же модель даже после смены её места в массиве

    Model* selectedModel = m_models[m_selectedModelIndex];



    Model** modelArr = new Model * [m_models.size()];
    int modelSize = m_models.size();
    for (int i = 0; i < m_models.size(); i++) {
        modelArr[i] = m_models[i];
    }
    qsortRecursive(modelArr, m_models.size());

    m_models.clear();
    for (int i = 0; i < modelSize; i++) {
        m_models.push_back(modelArr[i]);
        if (modelArr[i] == selectedModel) {
            m_selectedModelIndex = i;
        }
    }

    delete[] modelArr;

	m_namesCstr.clear();

	for (int i = 0; i < m_models.size(); i++) {
		m_namesCstr.push_back(m_models[i]->m_name);
	}
}